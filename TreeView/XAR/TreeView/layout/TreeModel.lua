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
		rootDir = "./"
	end

	local virtualRoot = createNode("", "")
	local root = createNode(rootDir, rootDir)
	root.Father = virtualRoot
	root.Expand = true
	root.HasChildren = true
	virtualRoot.Children = {}
	virtualRoot.Children.Root = root
	virtualRoot.Expand = true
	virtualRoot.HasChildren = true

	local dirFactory = XLGetObject("LuaDir.Factory.Object")
	local dir = dirFactory:CreateInstance()
	-- local subDirs = dir:GetSubDirs(rootDir)
	-- for k,info in pairs(subDirs) do
		-- local from, to = string.find(info[2], ".\\")
		-- local key = rootDir..(string.sub(info[2], to+1, -1))
		-- local value=string.sub(info[2], to+1, -1)
		-- local node = createNode(key, value)
		-- node.Father = root
		-- node.Expand = false
		-- root.Children[key] = node
		-- if info[1] == true then
			-- node.HasChildren = true
		-- else node.HasChildren = false end
	-- end
	
	treeModel.GetChildren = 
		function (Node)
			if Node.Key ~= "" then 
				Log("get children:"..Node.Key)
				local curDir = Node.Key
				local subDirs = dir:GetSubDirs(curDir)
				for k,info in pairs(subDirs) do
					local from, to = string.find(info[2], ".\\")
					local value = info[2]
					if to then value=string.sub(info[2], to+1, -1) end
					local key = curDir..value
					local node = createNode(key, value)
					node.Father = Node
					node.Expand = false
					Node.Children[key] = node
					if info[1] == true then
						node.HasChildren = true
					else node.HasChildren = false end
				end
			end
			return Node.Children
		end

	treeModel.HasChildren = 
		function (node)
			return node.HasChildren
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