
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
--long result,bool bHandled,bool callNextEvent OnLButtonDown(UIObject sender,int x,int y,int flags)
function OnLButtonDown(self, x, y, flags)
	local logObjFactory = XLGetObject("LogObjectFactory")
	if logObjFactory then
		log = logObjFactory:CreateInstance()
		log:Log(123)
	end
	return 0, true, true
end