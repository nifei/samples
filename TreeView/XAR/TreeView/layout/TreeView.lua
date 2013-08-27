-- TreeView structure
-- Node.Key			string
-- Node.Object		UIObject
-- Node.Children	table of Node
-- Node.Size		table{int w, int h}, sub tree rect size
-- Node.Expand		bool
-- Node.Father?		Node
-- Node.Data?		userdata

-- TreeView method
function createNode(key, data)
	local Node={}
	Node.Key=key
	Node.Children={}
	Node.Object=nil
	Node.Size={}
	Node.Expand=true
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
		Node.Children = nodes
		return Node
	end
	if type(nodeTable) ~= "table" then
		local Node = createNode(key, nodeTable)
		return Node
	end
end

function RenderTreeFromDataTable(self, dataTable)
	local root = createNodeTreeFromTable("nil", dataTable)
end

-- tree method
-- in father object size, children subtree size list
-- out children pos list relative to father left top

-- client Method
function CreateNodeObject(data)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	objFactory:CreateUIObject(nil, "TreeView.Node")
end

-- client method
-- in sizelist
-- out poslist
-- 位置是相对fatherObj左上角的
function GetChildrenPosList(fatherObjSize, childrenSubTreeSizeList)
end
