local mdDataModel = XLLoadModule(lua_code_dir.."TestDataModel.lua")
local GetTestDataModelObject = mdDataModel.GetTestDataModelObject

local mdItemFactory = XLLoadModule(lua_code_dir.."ItemFactory.lua")
local GetItemFactory = mdItemFactory.GetItemFactory

function OnInitControl_ItemView(self)
	-- Setup item factory
	local itemFactoryUserData, itemFactoryCallbackTable = GetItemFactory()
	self:SetItemFactory(itemFactoryUserData, nil, itemFactoryCallbackTable)

	-- Setup data model,  by default lua table is applied. 
	-- -- 示例代码 Custom data model import by "require"
	local dataModelUserData, dataModelCallbackTable = GetTestDataModelObject("simple.xml")
	self:SetDataModel(dataModelUserData, dataModelCallbackTable)
	
	-- -- user can: 1. specify another lua table as data model;  
	-- -- 示例代码： Local table data model
	-- local dt = {{"cats"},{"eats"},{"fish"}}
	-- self:SetDataTable(dt)
	
	-- 示例代码set header data
	self:SetHeaderNameList({"Cover", "Name", "Dir"})
end

function scroll_down_OnLButtonDown(self)
	local parent = self:GetParent():GetParent()
	local count = parent:GetChildCount()
	for i=0,count-1 do
		local obj = parent:GetChildByIndex(i)
		if obj:GetID() == "itemView" then
			local scroll = obj
			scroll:PageDown()
		end
	end
end

function scroll_up_OnLButtonDown(self)
	local parent = self:GetParent():GetParent()
	local count = parent:GetChildCount()
	
	for i=0,count-1 do
		local obj = parent:GetChildByIndex(i)
		if obj:GetID() == "itemView" then
			local scroll = obj
			scroll:PageUp()
		end
	end
end

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