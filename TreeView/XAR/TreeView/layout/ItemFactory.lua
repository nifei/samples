function CreateNodeObject(data,key)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Node")
	local text = object:GetObject("text")
	if text then 
		text:SetText(data)
	end
	local size = {width=math.ceil(string.len(data)*7),height=20}
	return object,size
end

-- clinet method
-- expected method:SetLinePoint
function CreateLineObject(k)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local line = objFactory:CreateUIObject(nil, "LineObject")
	line:SetVisible(true)
	line:SetLinePenResID("pen.dot")
	line:SetLineColorResID("system.black")
	line:SetObjPos2(0,0, "father.width", "father.height")
	return line
end

function GetCustomItemFactory(tree)
	local itemFactory = {}
	itemFactory.CreateNodeObject = CreateNodeObject
	itemFactory.CreateLineObject = CreateLineObject
	return itemFactory
end