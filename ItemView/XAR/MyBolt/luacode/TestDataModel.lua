function GetCount(userdata)
	return userdata.class:GetCount(userdata.name)
end

function GetItemAtIndex(userdata, row, column)
	return userdata.class:GetItemAtIndex(userdata.name, row, column)
end

function GetTestDataModelObject(name)
	local xmlClassFactory = XLGetObject("XmlDataModel.Factory")
	-- local xmlClass = xmlClassFactory:CreateInstance(name)
	local xmlClass = xmlClassFactory:CreateInstance({name, "2", "other string information"})
	local userdata ={}
	userdata.class = xmlClass
	userdata.name = name
	local callbackTable = {}
	callbackTable.GetCount = GetCount
	callbackTable.GetItemAtIndex = GetItemAtIndex
	callbackTable.GetDataBatch = 
		function (userdata, from, to)
			local dataBatch = userdata.class:GetDataBatch(userdata.name, from, to)
			return dataBatch
		end
	callbackTable.GetColumnCount = 
		function(userdata)
			return userdata.class:GetColumnCount(userdata.name)
		end
	callbackTable.SetDataBatchReadyListener = 
		function (userdata, dataBatchReadyListener)
			userdata.class:AttachDataBatchReadyListener(dataBatchReadyListener)
		end
		
	callbackTable.SetSingleDataReadyListener = 
		function(userdata, singleDataReadyListener)
			userdata.class:AttachSingleDataReadyListener(singleDataReadyListener)
		end
		
	callbackTable.PrepareData = 
		function(userdata, from, to)
			userdata.class:PrepareData(userdata.name, from, to)
		end
	callbackTable.ReleaseData = 
		function(userdata, from, to)
			userdata.class:ReleaseData(userdata.name, from, to)
		end
	return userdata, callbackTable
end