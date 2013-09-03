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
	tree.A1 = "A"
	tree.B1 = "B"
	local subTree = {}
	subTree.C = "C"
	subTree.D = "D"
	tree.E = subTree
	local subTree2 = {}
	subTree2.F = "E"
	subTree2.G = "F"
	subTree.H = subTree2
	tree.C = subTree2
	tree.D = subTree
	return {root=tree}
end

function OnInitTree(tree)
	local dataTable = createDataTreeTable()
	tree:RenderTreeFromDataTable(dataTable)
end

function OnTest(layout)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	
	local line = objFactory:CreateUIObject(nil, "LineObject")
	layout:AddChild(line)
	line:SetVisible(true)
	line:SetLinePenResID("pen.dot")
	line:SetLineColorResID("system.black")
	line:SetObjPos(100,100,200,200)
	local l,t,r,b = line:GetAbsPos()
	Log("l:"..l.." t:"..t.." r:"..r.." b:"..b)
	line:SetLinePoint(0,0,"width","height")
	
	local rect = objFactory:CreateUIObject(nil, "TreeView.Node")
	layout:AddChild(rect)
	rect:SetObjPos(100,100,200,200)
end