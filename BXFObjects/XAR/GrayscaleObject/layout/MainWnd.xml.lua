function OnSize(self, sizetype, width, height)
	if type_ == "min" then
		return
	end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, width, height)
	
end

function OnMouseWheel(self, x, y, distance, flags)
	return 0, true, true
end

function OnClose(self)
	--UELoader的全局函数
	Quit()
end