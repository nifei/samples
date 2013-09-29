
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

local derived1 = nil
local derived2 = nil
local base = nil

--long result,bool bHandled,bool callNextEvent OnLButtonDown(UIObject sender,int x,int y,int flags)
function OnLButtonDown(self, x, y, flags)
	local dataClassFactory = XLGetObject("LuaBaseFactoryObject")
    base = base or dataClassFactory:CreateInstance("Base")
    derived1 = derived1 or dataClassFactory:CreateInstance("Derived1")
    derived2 = derived2 or dataClassFactory:CreateInstance("Derived2")
	if self:GetClass() == "TextObject" then 
		local id = self:GetID()
		if id == "Base" then 
			base:Click()
		end
		if id == "Derived1" then
			derived1:Click()
			derived1:Click1()
		end
		if id == "Derived2" then
			derived2:Click()
			derived2:Click2()
			derived2:Click3()
		end
	end
	return 0, true, true
end