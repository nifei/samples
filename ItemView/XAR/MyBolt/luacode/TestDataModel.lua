function GetXmlDataModelObject(dataModelClassName, playlistFileName)
	local xmlClassFactory = XLGetObject(dataModelClassName..".Factory"..".Object")
	local xmlClass = xmlClassFactory:CreateInstance(dataModelClassName, playlistFileName)
	local userdata ={}
	userdata.class = xmlClass
	userdata.className = dataModelClassName
	local callbackTable = {}
	callbackTable.GetCount = 
		function(userdata)
			return userdata.class:GetCount(userdata.className)
		end
	callbackTable.GetItemAtIndex = 
		function(userdata, row, column)
			return userdata.class:GetItemAtIndex(userdata.className, row, column)
		end
	callbackTable.GetDataBatch = 
		function (userdata, from, to)
			local dataBatch = userdata.class:GetDataBatch(userdata.className, from, to)
			return dataBatch
		end
	callbackTable.GetColumnCount = 
		function(userdata)
			return userdata.class:GetColumnCount(userdata.className)
		end
	-- callbackTable.SetDataBatchReadyListener = 
		-- function (userdata, dataBatchReadyListener)
			-- userdata.class:SetDataBatchReadyListener(userdata.className, dataBatchReadyListener)
		-- end
		
	callbackTable.SetSingleDataReadyListener = 
		function(userdata, singleDataReadyListener)
			userdata.class:SetSingleDataReadyListener(userdata.className, singleDataReadyListener)
		end
		
	callbackTable.PrepareData = 
		function(userdata, from, to)
			userdata.class:PrepareData(userdata.className, from, to)
		end
	callbackTable.ReleaseData = 
		function(userdata, from, to)
			userdata.class:ReleaseData(userdata.className, from, to)
		end
	return userdata, callbackTable
end