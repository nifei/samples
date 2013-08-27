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

-- Client Method
function CreateNodeObject(data)
end

-- client method
-- in sizelist
-- out poslist
function GetChildrenPosList(fatherObj, sizeList)
end

-- client method
function GetLinesObject(fatherObj, posList)
end

-- client dataTable Method
function createDataTreeTable()
	local tree={}
	local firstChild = {}
	firstChild.A = "leafA"
	firstChild.B = "leafB"
	tree.root = {}
	tree.root.firstChild = firstChild
	local secondChild = {}
	secondChild.A = "leafA2"
	secondChild.B = "leafB2"
	tree.root.secondChild = secondChild
	return tree
end

function OnInitControlTree(self)
	local dataTable = createDataTreeTable()
	self:RenderTreeFromDataTable(dataTable)
end