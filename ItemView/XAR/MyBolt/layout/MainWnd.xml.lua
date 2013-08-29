local mdDataModel = XLLoadModule(lua_code_dir.."TestDataModel.lua")
local GetXmlDataModelObject = mdDataModel.GetXmlDataModelObject
local GetSimpleDataModelObject = mdDataModel.GetSimpleDataModelObject

local mdItemFactory = XLLoadModule(lua_code_dir.."ItemFactory.lua")
local GetItemFactory = mdItemFactory.GetItemFactory

local mdTextItemFactory = XLLoadModule(lua_code_dir.."SimpleItemFactory.lua")
local GetTextItemFactory = mdTextItemFactory.GetTextItemFactory

function OnInitControl_ItemView(self)
	initItemViewWithSimpleModel(self)
end

local SimpleDataModelOperation = nil
function initItemViewWithSimpleModel(self)
	local itemFactoryUserData, itemFactoryCallbackTable = GetTextItemFactory()
	self:SetItemFactory(itemFactoryUserData, nil, itemFactoryCallbackTable)
	
	local dataModelUserData, dataModelCallbackTable, operation = GetSimpleDataModelObject()
	self:SetDataModel(dataModelUserData, dataModelCallbackTable)
	SimpleDataModelOperation = operation
end

function insert()
	if SimpleDataModelOperation ~= nil then
		SimpleDataModelOperation.Add(nil, os.date())
	end
end

function remove_()
	if SimpleDataModelOperation ~= nil then
		SimpleDataModelOperation.Sub(nil)
	end
end

function update()
	if SimpleDataModelOperation ~= nil then
		SimpleDataModelOperation.Change(nil, "update")
	end
end


function initItemViewWithCppModel(self)
	-- Setup item factory
	local itemFactoryUserData, itemFactoryCallbackTable = GetItemFactory()
	self:SetItemFactory(itemFactoryUserData, nil, itemFactoryCallbackTable)

	-- Setup data model,  by default lua table is applied. 
	-- -- 示例代码 Custom data model import by "require"
	local dataModelUserData, dataModelCallbackTable = GetXmlDataModelObject("XmlDataModel", "simple.xml")
	self:SetDataModel(dataModelUserData, dataModelCallbackTable)
	
	-- -- user can: 1. specify another lua table as data model;  
	-- -- 示例代码： Local table data model
	-- local dt = {{"cats"},{"eats"},{"fish"}}
	-- self:SetDataTable(dt)
	
	-- 示例代码 set header data
	self:SetHeaderNameList({"Cover", "Name", "Dir"})
	
	local function OnItemViewHorizontalScrollPosChanged(itemViewObject, itemViewEventName, oldPos, newPos)
		-- 监听自定义控件自定义事件方法二:
		-- 这样做的好处是不想继续监听的时候可以根据AttachListener返回的cookie移除监听
	end
	local cookie, ret = self:AttachListener("HorizontalScrollPosChanged", true, OnItemViewHorizontalScrollPosChanged)
	if ret then
		-- 移除对事件HorizontalScrollPosChanged的监听, 所以上面的AttachListener不再有效
		self:RemoveListener("HorizontalScrollPosChanged", cookie)
	end
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