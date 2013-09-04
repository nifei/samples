local treeModel = {}

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
		local nodes = {}
		for k,v in pairs(nodeTable) do
			local newKey = key.."."..k
			local node=createNodeTreeFromTable(newKey, v)
			node.Father = Node
			nodes[newKey]=node
		end
		Node.Children = nodes
		Node.Data = key
		return Node
	end
	if type(nodeTable) ~= "table" then
		local Node = createNode(key, key)
		return Node
	end
end

function GetTreeModelFromTable(dataTable)
	local root = createNodeTreeFromTable("", dataTable)
	root.Expand = true
	local i = 1
	for k,v in pairs(root.Children) do
		v.Expand = true
	end
	
	treeModel.GetChildren = 
		function (node)
			return node.Children
		end
	treeModel.HasChildren = 
		function (node)
			for k,v in pairs(node.Children) do
				return true
			end
			return false
		end
	treeModel.GetFather = 
		function (node)
			return node.Father
		end
	treeModel.GetChild = 
		function (node, key)
			return node.Children[key]
		end
	treeModel.GetRoot = 
		function()
			return treeModel.Root
		end
	treeModel.Root = root
	return treeModel
end

function GetDirTreeModel(rootDir)
	if not rootDir then 
		local path = __document
		local index = string.find(path, "/[^/]*$")
		rootDir = string.sub(path,1,index)
	end
	os.execute('dir ' .. rootDir .. ' /s > temp.txt')
	--io.input("temp.txt")
	local virtualRoot = createNode("", "")
	local root = createNode("root", rootDir)
	root.Father = virtualRoot
	virtualRoot.Children = {}
	virtualRoot.Children.Root = root
	virtualRoot.Expand = true
	treeModel.GetChildren = 
		function (node)
			return node.Children
		end
	treeModel.HasChildren = 
		function (node)
			for k,v in pairs(node.Children) do
				return true
			end
			return false
		end
	treeModel.GetFather = 
		function (node)
			return node.Father
		end
	treeModel.GetChild = 
		function (node, key)
			return node.Children[key]
		end
	treeModel.GetRoot = 
		function()
			return treeModel.Root
		end
	treeModel.Root = virtualRoot
	return treeModel
end