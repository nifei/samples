function close_btn_OnLButtonDown(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local frameHostWnd2 = hostwndManager:GetHostWnd("MainFrame")
	frameHostWnd2:Destroy()
	os.exit()
end

function OnMainWndSize(self, type_, width, height)
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, width, height)
end

-- client dataTable Method
function createDataTreeTable()
	local tree={}
	tree.A = "A"
	tree.B = "B"
	local subTree = {}
	subTree.C = "C"
	subTree.D = "D"
	tree.E = subTree
	local subTree2 = {}
	subTree2.F = "F"
	subTree2.G = "G"
	subTree.H = subTree2
	return {root=tree}
end

function OnInitTree(tree)
	local dataTable = createDataTreeTable()
	tree:RenderTreeFromDataTable(dataTable)
end