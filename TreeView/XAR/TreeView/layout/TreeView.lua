-- TreeView structure
-- Node.Key			string
-- Node.Object		UIObject
-- Node.Children	table of Node
-- Node.Size		table{int w, int h}, sub tree rect size
-- Node.Expand		bool
-- Node.Father?		Node
-- Node.Data?		userdata

-- event oninitcontrol --

-- TreeView method
function createNode(key, data)
	local Node={}
	Node.Key=key
	Node.Children={}
	Node.Object=nil
	Node.Layout=nil
	Node.Size={width=0,height=0}
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
	canvas:SetObjPos2(0, 0, rootNode.Size.width, rootNode.Size.height)
end

-- return Node with object and layout
function initNode(Node)
	if Node.Object ==  nil then 
		Node.Object = CreateNodeObject(Node.Data) 
		Node.Size = {width=60, height=20}
	end
	if Node.Layout ==  nil and #(Node.Children)>0 then
		Node.Layout = createNodeLayout(Node)
		local sizeList = {}
		for k,v in pairs(Node.Children) do
			if v.Object == nil then
				Node.Children[k] = initNode(v)
			end
			sizeList[k] = Node.Children[k].Size
		end
		local fatherPos, poslist = GetChildrenPosListWindowsStyle(Node.Size, sizeList)
		local width = Node.Size.width
		local height = Node.Size.height
		for k,v in pairs(Node.Children) do
			local NodeObject = Node.Children[k].Layout and Node.Children[k].Layout or Node.Children[k].Object
			Node.Layout:AddChild(NodeObject)
			local w = Node.Children[k].Size.width
			local h = Node.Children[k].Size.height
			local pos = poslist[k]
			NodeObject:SetObjPos2(pos.left, pos.top, w, h)
			if width < pos.left+w then width = pos.left+w end
			if height < pos.top+h then height = pos.top+h end
		end
		Node.Layout:AddChild(Node.Object)
		Node.Object:SetObjPos(fatherPos.left, fatherPos.top, Node.Size.width, Node.Size.height)
		Node.Size = {width=width, height=height}
	end
	return Node
end
-- return layout
function createNodeLayout(Node)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local layout = objFactory:CreateUIObject(nil, "LayoutObject")
	layout:SetVisible(false)
	return layout
end

-- tree method
-- in father object size, children subtree size list
-- out children pos list relative to father left top

-- client Method
function CreateNodeObject(data)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Node")
	return object
end

-- client method
-- in size+sizelist
-- out pos + poslist
-- 位置是相对fatherObj左上角的
function GetChildrenPosList(fatherObjSize, childrenSubTreeSizeList)
	local poslist = {}
	local left = fatherObjSize.width + 10
	local top = fatherObjSize.height
	for k,v in pairs(childrenSubTreeSizeList) do
		local size = childrenSubTreeSizeList[k]
		poslist[k] ={left=left, top=top}
		top = top+size.height
	end
	return {left=0,top=0},poslist
end

function GetChildrenPosListLeftMidFather(fatherObjSize, sizelist)
	local poslist = {}
	local left = fatherObjSize.width + 10
	local height = 0
	for k,v in pairs(sizelist) do
		height = height + v.height
	end
	local top = 0
	for k,v in pairs(sizelist) do
		local size = sizelist[k]
		poslist[k] ={left=left, top=top}
		top = top+size.height
	end
	return {left=0, top=top/2-fatherObjSize.height/2}, poslist
end

function GetChildrenPosListTopMidFather(fsize, sizelist)
	local poslist = {}
	local left = 0
	local top = fsize.height
	local height = 0
	local width = 0
	local padding = 10
	for k,v in pairs(sizelist) do
		local size = sizelist[k]
		poslist[k] ={left=left, top=top}
		left = left + size.width + padding
		width = width + v.width + padding
	end
	width = width-padding
	return {left=width/2-fsize.width/2, top=0}, poslist
end

function GetChildrenPosListWindowsStyle(fsize, sizelist)
	local poslist = {}
	local indent = 50 -- todo Style attribute
	local left = indent
	local padding = 10
	local top = padding + fsize.height
	for k,v in pairs(sizelist) do
		poslist[k] = {left=left, top=top}
		top = top + sizelist[k].height+padding
	end
	return {left=0,top=0},poslist
end