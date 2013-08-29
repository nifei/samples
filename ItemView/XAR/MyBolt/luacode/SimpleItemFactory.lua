-- -- This is provided by user
function GetTextItemFactory()
	local callbackTable = {}
	local userdata = nil
	callbackTable.CreateObject = 
		function (userdata, column)
			local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			local itemObj = objFactory:CreateUIObject(nil,"TextObject")
			return itemObj
		end
	callbackTable.GetRowHeight = 
		function (userdata)
			return 50
		end
	callbackTable.GetColumnWidth = 
		function (userdata)
			return 200
		end
	callbackTable.SetItemData = 
		function(userdata, itemObj, data, row, column)
			if itemObj == nil then return end
			if itemObj:GetClass() == "TextObject" then
				if data == nil then data = "" end
				itemObj:SetText(data)
			end
		end
	return userdata, callbackTable
end