-- TreeView structure
-- Node.Key			string
-- Node.Object		UIObject
-- Node.Children	table of Node
-- Node.LayoutSize		table{int w, int h}, sub tree rect size
-- Node.JointObject
-- Node.Expand		bool
-- Node.Father?		Node
-- Node.Data?		userdata

local TVStyle=nil	

-- event OnBind --
function OnBind(tree)
	local path = __document
	local index = string.find(path, "/[^/]*$")
	local layoutDir = string.sub(path,1,index)
	local styleFile = layoutDir.."TreeViewStyle.lua"
	local mdStyle = XLLoadModule(styleFile)
	mdStyle.RegisterGlobal()
	TVStyle=GetTreeStyle(tree)
	return 0, true
end

-- TreeView method
function createNode(key, data)
	local Node={}
	Node.Key=key
	Node.Children={}
	Node.Object=nil
	Node.Layout=nil
	Node.ObjectSize = nil
	Node.LayoutSize=Node.ObjectSize
	Node.Data = data
	Node.Expand =false
	Node.Lines = {}
	return Node
end

function createNodeTreeFromTable(key, nodeTable)
	if nodeTable == nil then return nil end
	if type(nodeTable) == "table" then
		local Node = createNode(key)
		local i = 1
		local nodes = {}
		for k,v in pairs(nodeTable) do
			local node=createNodeTreeFromTable(k,v)
			node.Father = Node
			nodes[i]=node
			i = i+1
		end
		Node.Children = nodes
		Node.Data = key
		return Node
	end
	if type(nodeTable) ~= "table" then
		local Node = createNode(key, nodeTable)
		return Node
	end
end

function RenderTreeFromDataTable(tree, dataTable)
	local root = createNodeTreeFromTable("o", dataTable)
	root.Expand = true
	for k,v in pairs(root.Children) do
		v.Expand = true
	end
	root.Children[1].Children[2].Expand = true
	tree:GetAttribute().RootNode = root
	RenderTreeFromNode(tree, tree:GetAttribute().RootNode)
end

function RenderTreeFromNode(tree, rootNode)
	rootNode = initNode(rootNode, tree)
	local canvas = rootNode.Layout and rootNode.Layout or rootNode.Object
	local scrollpanel = tree:GetObject("tree.scroll")
	scrollpanel:SetInnerObject(canvas)
	canvas:SetObjPos2(0, 0, rootNode.LayoutSize.width, rootNode.LayoutSize.height)
	-- canvas:AddChild(rootNode.JointObject)
	rootNode.JointObject:SetObjPos(0,0,20,20)
end

function rmvObj(obj)
	if obj and obj:GetFather() then
		obj:GetFather():RemoveChild(obj)
	end
	return nil
end

function releaseNode(Node)
	Node.Object = rmvObj(Node.Object)
	Node.Layout = rmvObj(Node.Layout)
	Node.JointObject = rmvObj(Node.JointObject)
	for k, line in pairs(Node.Lines) do
		rmvObj(line)
	end
	Node.Lines = {}
	for k,node in pairs(Node.Children) do
		Node.Children[k] = releaseNode(Node.Children[k])
	end
	return Node
end

-- return Node with object and layout
function initNode(Node, treeView)
	if Node.Object ==  nil then 
		Node.Object, Node.ObjectSize = CreateNodeObject(Node.Data)
		if Node.ObjectSize == nil then 
			Node.ObjectSize = TVStyle.node_size
		end
		if not Node.Father then 
			Node.ObjectSize = {width = 0, height = 0}
		end
		Node.Layout = createNodeLayout(Node)
		Node.LayoutSize = TVStyle.node_size
		Node.Layout:AddChild(Node.Object)
		Node.Object:SetObjPos(0, 0, Node.ObjectSize.width, Node.ObjectSize.height)
		if #(Node.Children) > 0 and not Node.JointObject then 
			Node.JointObject = CreateJointObject()
			local OnJointTriggered = 
				function (joint, eventName, expand)
					local prevState = Node.Expand
					Node.Expand = expand
					if prevState ~= expand then 
						Node = releaseNode(Node)
						local father = Node
						while father.Father do
							father = father.Father
						end
						father = initNode(father)
						father.Layout:SetObjPos2(0, 0, father.LayoutSize.width, father.LayoutSize.height)
					end
				end
			Node.JointObject:AttachListener("Triggered", true, OnJointTriggered)
		end
		Node.Dirty = true
	end
	if #(Node.Children)>0 and Node.Expand then
		local sizeList = {}
		for k,v in pairs(Node.Children) do
			Node.Children[k] = initNode(v)
			sizeList[k] = Node.Children[k].LayoutSize
		end
		local fatherPos, poslist = TVStyle.GetChildrenPosList(Node.ObjectSize, sizeList)
		local width = Node.ObjectSize.width
		local height = Node.ObjectSize.height
		local rectlist = {}
		for k,v in pairs(Node.Children) do
			if Node.Children[k].Dirty then
				Node.Children[k].Dirty = false
				Node.Dirty = true
			end
			if Node.Dirty then
				local w = Node.Children[k].LayoutSize.width
				local h = Node.Children[k].LayoutSize.height
				local pos = poslist[k]
				Node.Children[k].Layout:SetObjPos2(pos.left, pos.top, w, h)
				rectlist[k] = {left = pos.left, top = pos.top, width = Node.Children[k].ObjectSize.width, height = Node.Children[k].ObjectSize.height}
				if width < pos.left+w then width = pos.left+w end
				if height < pos.top+h then height = pos.top+h end
				if not Node.Children[k].Layout:GetFather() then 
					Node.Layout:AddChild(Node.Children[k].Layout)
				end
			end
		end
		
		if Node.Dirty then
			Node.Object:SetObjPos(fatherPos.left, fatherPos.top, Node.ObjectSize.width, Node.ObjectSize.height)
			Node.LayoutSize = {width=width, height=height}
			
			local frect = {left = fatherPos.left, 
							top = fatherPos.top, 
							width = Node.ObjectSize.width, 
							height = Node.ObjectSize.height}
			
			local linePosList, jointPosList = TVStyle.GetLinesAndJointPosList(frect, rectlist, not Node.Father)
			if Node.Layout then
				for k,pos in pairs(linePosList) do
					if Node.Lines[k] == nil then 
						Node.Lines[k] = CreateLineObject(k)
						Node.Layout:AddChild(Node.Lines[k])
					end
					Node.Lines[k]:SetLinePoint(pos.left, pos.top, pos.right, pos.bottom)
				end
				for k,pos in pairs(jointPosList) do
					if Node.Children[k].JointObject then
						if not Node.Children[k].JointObject:GetFather() then
							Node.Layout:AddChild(Node.Children[k].JointObject)
						end
						Node.Children[k].JointObject:SetObjPos(pos.left, pos.top, pos.left+pos.width, pos.top+pos.height)
						Node.Children[k].JointObject:SetExpand(Node.Children[k].Expand)
					end
				end
			end
		end
	else
		for k, node in pairs(Node.Children) do
			Node.Children[k] = releaseNode(Node.Children[k])
		end
	end
	return Node
end

-- treeview method
-- return layout
function createNodeLayout(Node)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local layout = objFactory:CreateUIObject(nil, "LayoutObject")
	layout:SetVisible(false)
	return layout
end

-- clinet method
function CreateJointObject(data)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Joint")
	local text = object:GetObject("text")
	return object
end

-- client Method
function CreateNodeObject(data)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Node")
	local text = object:GetObject("text")
	if text then 
		text:SetMultiline(true)
		text:SetText(data)
	end
	return object
end

-- clinet method
function CreateLineObject(k)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local line = objFactory:CreateUIObject(nil, "LineObject")
	line:SetVisible(true)
	--line:SetZorder(1000)
	line:SetLinePenResID("pen.dot")
	line:SetLineColorResID("system.black")
	line:SetObjPos2(0,0, "father.width", "father.height")
	return line
end