function GetSimpleDataModelObject()
	local dataTable = {{":)", "T.T"}}
	local callbackTable = {}
	local operation = {}
	callbackTable.GetCount = 
		function (userdata)
			return #dataTable
		end
	callbackTable.GetColumnCount=
		function (userdata)
			if dataTable[1] ~= nil then return #(dataTable[1]) 
			else return 1 end
		end
	callbackTable.GetItemAtIndex = 
		function (userdata, row, column)
			if dataTable[row] ~= nil then
				return dataTable[row][column]
			else
				return nil
			end
		end
	callbackTable.SetDataChangeListener = 
		function (userdata, dataChangedListener)
			callbackTable.DataChangeListener = dataChangedListener
		end
	operation.Add = 
		function (userdata, data)
			local count = #dataTable
			dataTable[count+1]={"inserted:", data}
			if callbackTable.DataChangeListener ~= nil then
				callbackTable.DataChangeListener(count+1, count+1)
			end
		end
	
	operation.Sub = 
		function (userdata)
			local count = #dataTable
			if count > 0 then
				for row=1,count do
					dataTable[row] = dataTable[row+1]
				end
			end
			if callbackTable.DataChangeListener ~= nil then
				callbackTable.DataChangeListener(1, 1)
			end
		end
	
	operation.Change = 
		function (userdata, data)
			local count = #dataTable
			dataTable[1]= {"update:", data}
			if callbackTable.DataChangeListener ~= nil then
				callbackTable.DataChangeListener(1, 1)
			end
		end
	return nil, callbackTable, operation
end

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