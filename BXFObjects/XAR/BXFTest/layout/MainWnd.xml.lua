function OnSize(self, sizetype, width, height)
	if type_ == "min" then
		return
	end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, width, height)
	
end

function OnMouseWheel(self, x, y, distance, flags)
	local oldSigma = self:GetSigma()
	local newSigma = oldSigma + distance/500
	if newSigma <= 0 then newSigma = 0 end
	self:SetRadius(3*newSigma)
	self:SetSigma(newSigma)
	return 0, true, true
end

function OnClose(self)
	--UELoader的全局函数
	Quit()
end