-- clinet method
-- expected event :Triggered
function CreateJointObject(data)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Joint")
	local text = object:GetObject("text")
	return object
end

function CreateNodeObject(data,key)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local object = objFactory:CreateUIObject(nil, "TreeView.Node")
	local text = object:GetObject("text")
	if text then 
		text:SetText(data)
	end
	local size = {width=string.len(data)*6,height=20}
	return object,size
end

-- clinet method
-- expected method:SetLinePoint
function CreateLineObject(k)
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local line = objFactory:CreateUIObject(nil, "LineObject")
	line:SetVisible(true)
	--line:SetZorder(1000)
	line:SetLinePenResID("pen.dot")
	line:SetLineColorResID("system.black")
	line:SetObjPos2(0,0, "father.width", "father.height")
	return line
end

function GetCustomItemFactory(tree)
	local itemFactory = {}
	itemFactory.CreateJointObject = CreateJointObject
	itemFactory.CreateNodeObject = CreateNodeObject
	itemFactory.CreateLineObject = CreateLineObject
	return itemFactory
end