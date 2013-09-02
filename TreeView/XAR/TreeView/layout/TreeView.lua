-- TreeView structure
-- Node.Key			string
-- Node.Object		UIObject
-- Node.Children	table of Node
-- Node.LayoutSize		table{int w, int h}, sub tree rect size
-- 
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
	return Node
end

function createNodeTreeFromTable(key, nodeTable)
	if nodeTable == nil then return nil end
	if type(nodeTable) == "table" then
		local nodes = {}
		local Node = createNode(key)
		local i = 1
		for k,v in pairs(nodeTable) do
			local node=createNodeTreeFromTable(k,v)
			node.Father = Node
			nodes[i]=node
			i = i+1
		end
		Node.Data = key
		Node.Children = nodes
		return Node
	end
	if type(nodeTable) ~= "table" then
		local Node = createNode(key, nodeTable)
		return Node
	end
end

function RenderTreeFromDataTable(tree, dataTable)
	local root = createNodeTreeFromTable("virtual root", dataTable)
	tree:GetAttribute().RootNode = root
	RenderTreeFromNode(tree, tree:GetAttribute().RootNode)
end

function RenderTreeFromNode(tree, rootNode)
	rootNode = initNode(rootNode)
	local canvas = rootNode.Layout and rootNode.Layout or rootNode.Object
	local scrollpanel = tree:GetObject("tree.scroll")
	scrollpanel:SetInnerObject(canvas)
	canvas:SetObjPos2(0, 0, rootNode.LayoutSize.width, rootNode.LayoutSize.height)
end

-- return Node with object and layout
function initNode(Node)
	if Node.Object ==  nil then 
		Node.Object, Node.ObjectSize = CreateNodeObject(Node.Data) 
		if Node.ObjectSize == nil then 
			Node.ObjectSize = TVStyle.node_size
		end
		Node.LayoutSize = TVStyle.node_size
	end
	if Node.Layout ==  nil and #(Node.Children)>0 then
		Node.Layout = createNodeLayout(Node)
		local sizeList = {}
		for k,v in pairs(Node.Children) do
			if v.Object == nil then
				Node.Children[k] = initNode(v)
			end
			sizeList[k] = Node.Children[k].LayoutSize
		end
		local fatherPos, poslist = TVStyle.GetChildrenPosList(Node.ObjectSize, sizeList)
		local width = Node.LayoutSize.width
		local height = Node.LayoutSize.height
		local rectlist = {}
		for k,v in pairs(Node.Children) do
			local NodeObject = Node.Children[k].Layout and Node.Children[k].Layout or Node.Children[k].Object
			Node.Layout:AddChild(NodeObject)
			local w = Node.Children[k].LayoutSize.width
			local h = Node.Children[k].LayoutSize.height
			local pos = poslist[k]
			NodeObject:SetObjPos2(pos.left, pos.top, w, h)
			local l, t, r, b =Node.Children[k].Object:GetObjPos()
			rectlist[k] = {left = pos.left, top = pos.top, width = Node.Children[k].ObjectSize.width, height = Node.Children[k].ObjectSize.height}
			if width < pos.left+w then width = pos.left+w end
			if height < pos.top+h then height = pos.top+h end
		end
		Node.Layout:AddChild(Node.Object)
		Node.Object:SetObjPos(fatherPos.left, fatherPos.top, Node.LayoutSize.width, Node.LayoutSize.height)
		Node.LayoutSize = {width=width, height=height}
		if TVStyle.RenderLinesAndJoint ~= nil then
		else
			local frect = {left = fatherPos.left, top = fatherPos.top, width = Node.ObjectSize.width, height = Node.ObjectSize.height}
			RenderLinesAndJoint(TVStyle, frect, rectlist, Node.Layout)
		end
	end
	return Node
end

function RenderLinesAndJoint(style, frect, rectlist, layout)
	local linePosList, jointPosList = style.GetLinesAndJointPosList(frect, rectlist)
	if layout then
		for k,pos in pairs(linePosList) do
			local line = CreateLineObject()
			local idPrefix =oldId
			layout:AddChild(line)
			line:SetLinePoint(pos.left, pos.top, pos.right, pos.bottom)
		end
		for k,pos in pairs(jointPosList) do
			local joint = CreateJointObject()
			layout:AddChild(joint)
			joint.SetObjPos(pos.left, pos.top, pos.width, pos.height)
		end
	end
end

-- return layout
function createNodeLayout(Node)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local layout = objFactory:CreateUIObject(nil, "LayoutObject")
	layout:SetVisible(false)
	return layout
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
function CreateLineObject()
	-- <objtemplate id="Line.Template" class="LineObject" >
		-- <attr>
			-- <left>0</left>
			-- <top>0</top>
			-- <width>father.width</width>
			-- <height>father.height</height>
			-- <pen>pen.dot</pen>
			-- <color>system.black</color>
			-- <visible>1</visible>
		-- </attr>
		-- <children>
		-- </children>
	-- </objtemplate>
	-- local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
	-- local template = templateManager:GetTemplate("Line.Template", "ObjectTemplate")
	-- local line = template:CreateInstance(id)
	-- line:SetVisible(true)
	-- line:SetZorder(1000)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local line = objFactory:CreateUIObject(nil, "LineObject")
	line:SetVisible(true)
	--line:SetZorder(1000)
	line:SetLinePenResID("pen.dot")
	line:SetLineColorResID("system.black")
	line:SetObjPos2(0,0, "father.width", "father.height")
	return line
end