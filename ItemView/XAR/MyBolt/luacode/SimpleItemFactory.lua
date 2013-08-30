-- -- This is provided by user
function GetTextItemFactory()
	local callbackTable = {}
	local userdata = nil
	callbackTable.CreateObject = 
		function (userdata, column)
			local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			if column == 1 then 
				local button = objFactory:CreateUIObject(nil,"BaseUI.Button")
				local cookie, ret = button:AttachListener("OnClick", true, OnButtonClicked)
				if ret then Log("attached") else Warn("not attached") end
				return button
			else return objFactory:CreateUIObject(nil,"TextObject") end
		end
	callbackTable.GetRowHeight = 
		function (userdata)
			return 50
		end
	callbackTable.GetColumnWidth = 
		function (userdata, column, widthInAll)
			if column == 1 then return 50 end
			if column == 2 then return 250 end
		end
	callbackTable.SetItemData = 
		function(userdata, itemObj, data, row, column)
			if itemObj == nil then return end
			if data == nil then data = "" end
			itemObj:SetText(data)
		end
	callbackTable.SetItemPos2 = 
		function (userdata, itemObj, left, top, width, height)
			if itemObj~=nil and itemObj:GetClass()=="BaseUI.Button" then
				itemObj:SetObjPos2(left+10, top, 30, 30)
				end
			if itemObj~=nil and itemObj:GetClass()=="TextObject" then
				itemObj:SetObjPos2(left, top, width, height) end
		end
	return userdata, callbackTable
end

function OnButtonClicked(button)
	if button:GetClass() == "BaseUI.Button" then
		local opr = button:GetText()
		if opr == "+" then
		elseif opr == "-" then
		elseif opr == "x" then
		end
	end
end