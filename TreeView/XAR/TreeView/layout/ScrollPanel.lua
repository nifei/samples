-- event OnInitControl --
function OnInitScrollPanel(scrollpanel)
	local viewport = scrollpanel:GetAttribute().Viewport
	local hsb = scrollpanel:GetAttribute().HScrollBar
	local vsb = scrollpanel:GetAttribute().VScrollBar
	local sa = scrollpanel:GetAttribute().ScrollArea
	scrollpanel:SetLimitChild(true)
	alignObjects(viewport, sa, hsb, vsb)
end

-- event OnInitControl --
function OnInitScrollArea(sa)
	local scrollpanel = sa:GetFather()
	scrollpanel:GetAttribute().ScrollArea = sa
end

function OnMouseWheel(scrollpanel, x, y, distance)
	local vsb = scrollpanel:GetAttribute().VScrollBar
	vsb:SetScrollPos(vsb:GetScrollPos()-math.floor(distance/10), true)
	vsb:FireExtEvent("OnScrollPosChange", vsb:GetScrollPos())
end

-- event OnInitControl --
function OnInitViewport(viewport)
	local scrollpanel = viewport:GetFather()
	scrollpanel:GetAttribute().Viewport = viewport
end

-- event OnInitControl --
function OnInitHSB(hsb)
	local scrollpanel = hsb:GetFather()
	scrollpanel:GetAttribute().HScrollBar = hsb
end

-- event OnInitControl --
function OnInitVSB(vsb)
	local scrollpanel = vsb:GetFather()
	scrollpanel:GetAttribute().VScrollBar = vsb
end

-- method --
function SetHScrollBar(scrollpanel, hsb)
	if scrollpanel:GetAttribute().HScrollBar ~=  nil then
		scrollpanel:GetAttribute().HScrollBar:RemoveListener("OnScrollPosChange")
		scrollpanel:RemoveChild(scrollpanel:GetAttribute().HScrollBar)
	end
	scrollpanel:GetAttribute().HScrollBar = hsb
	scrollpanel:GetAttribute().HScrollBar:AttachListener("OnScrollPosChange", true, OnHScrollPosChange)
end

-- method --
function SetVScrollBar(scrollpanel, vsb)
	if scrollpanel:GetAttribute().VScrollBar ~=  nil then
		scrollpanel:GetAttribute().VScrollBar:RemoveListener("OnScrollPosChange")
		scrollpanel:RemoveChild(scrollpanel:GetAttribute().VScrollBar)
	end
	scrollpanel:GetAttribute().VScrollBar = vsb
	scrollpanel:GetAttribute().VScrollBar:AttachListener("OnScrollPosChange", true, OnVScrollPosChange)
end

-- method -- 
function SetInnerObject(scrollpanel, sa)
	if scrollpanel:GetAttribute().ScrollArea ~= nil then
		scrollpanel:RemoveChild(scrollpanel:GetAttribute().ScrollArea)
	end
	scrollpanel:GetAttribute().ScrollArea = sa
	local viewport = scrollpanel:GetAttribute().Viewport
	local hsb = scrollpanel:GetAttribute().HScrollBar
	local vsb = scrollpanel:GetAttribute().VScrollBar
	scrollpanel:GetAttribute().ScrollArea:AttachListener("OnPosChange", true, OnScrollAreaPosChange)
	scrollpanel:AddChild(sa)
end

-- event OnPosChange of scroll area --
function OnScrollAreaPosChange(sa, lo, to, ro, bo, ln, tn, rn, bn)
	if rn-ln==ro-lo and bn-tn==bo-to then return 0, true end
	local scrollpanel = sa:GetFather()
	local viewport = scrollpanel:GetAttribute().Viewport
	local hsb = scrollpanel:GetAttribute().HScrollBar
	local vsb = scrollpanel:GetAttribute().VScrollBar
	alignObjects(viewport, sa, hsb, vsb)
	return 0, true
end

-- event OnPosChange -- 
function OnScrollPanelPosChange(scrollpanel)
	local viewport = scrollpanel:GetAttribute().Viewport
	local hsb = scrollpanel:GetAttribute().HScrollBar
	local vsb = scrollpanel:GetAttribute().VScrollBar
	local sa = scrollpanel:GetAttribute().ScrollArea
	alignObjects(viewport, sa, hsb, vsb)
end

-- event OnScrollPosChange --
function OnHScrollPosChange(hscrollbar)
	local pos = hscrollbar:GetScrollPos()
	local scrollpanel = hscrollbar:GetObject("father")
	setScrollAreaHPos(scrollpanel, pos)
end

-- event OnScrollPosChange -- 
function OnVScrollPosChange(vscrollbar)
	local pos = vscrollbar:GetScrollPos()
	local scrollpanel = vscrollbar:GetObject("father")
	setScrollAreaVPos(scrollpanel, pos)
end

function alignObjects(viewport, sa, hs, vs)
	if viewport and sa and vs and hs then
		local lv, tv, rv, bv = viewport:GetObjPos()
		local lsa, tsa, rsa, bsa = sa:GetObjPos()
		local wv = rv-lv; local hv = bv-tv; local wsa = rsa-lsa; local hsa = bsa-tsa;
		if wv < wsa then
			if 0-lsa>wsa-wv then lsa=wv-wsa end
			hs:SetVisible(true)
			hs:SetChildrenVisible(true)
			hs:SetScrollRange(0, wsa-wv)
		else
			lsa=0
			hs:SetVisible(false) 
			hs:SetChildrenVisible(false) 
		end
		if hv < hsa then 
			if 0-tsa>hsa-hv then tsa=hv-hsa end
			vs:SetVisible(true)
			vs:SetChildrenVisible(true)
			vs:SetScrollRange(0, hsa-hv)
		else
			tsa=0
			vs:SetVisible(false)
			vs:SetChildrenVisible(false)
		end
		sa:SetObjPos2(lsa, tsa, wsa, hsa)
	end
end

function setScrollAreaHPos(scrollpanel, pos)
	local sa = scrollpanel:GetAttribute().ScrollArea
	if sa then
		local l,t,r,b = sa:GetObjPos()
		sa:SetObjPos(0-pos, t, (r-l)-pos, b)
	end
end

function setScrollAreaVPos(scrollpanel, pos)
	local sa = scrollpanel:GetAttribute().ScrollArea
	if sa then
		local l,t,r,b = sa:GetObjPos()
		sa:SetObjPos(l, 0-pos, r, (b-t)-pos)
	end
end