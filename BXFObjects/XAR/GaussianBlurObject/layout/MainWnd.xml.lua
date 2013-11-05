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
	local newSigma = oldSigma + distance/1000
	local oldRadius = self:GetRadius()
	if newSigma <= 0 then newSigma = 0 end
	local newRadius =  3 * newSigma;
	newSigma = newSigma - newSigma % 0.01;
	newRadius = newRadius - newRadius %1;
	
	self:SetRadius(newRadius)
	self:SetSigma(newSigma)
	
	local textObj = self:GetObject("father:gaussian.text")
	if textObj then textObj:SetText("模糊系数:"..newSigma.." 模糊半径:"..newRadius) end
	return 0, true, true
end

function OnClose(self)
	--UELoader的全局函数
	Quit()
end