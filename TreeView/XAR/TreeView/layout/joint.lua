function SetExpand(joint, expand)
	local attr = joint:GetAttribute()
	local prevState = attr.Expand
	joint:GetAttribute().Expand = expand
	local text = joint:GetObject("text")
	text:SetText(expand and "-" or "+")
	if prevState ~= expand then joint:FireExtEvent("Triggered", expand) end
end

function SetIcons(joint, expandIcon, collapseIcon)
	if expandIcon then
		joint:GetAttribute().ExpandIcon = expandIcon
	end
	if collapseIcon then
		joint:GetAttribute().CollapseIcon = collapseIcon
	end
end

function IsExpand(joint)
	return joint:GetAttribute().Expand
end

function OnLButtonUp(joint, x, y)
	if joint:GetAttribute().ButtonDown == true then
		joint:GetAttribute().ButtonDown = false
		joint:SetExpand(not (joint:IsExpand()))
	end
end

function OnLButtonDown(joint, x, y)
	joint:GetAttribute().ButtonDown = true
end

function OnInitControl(joint)
	joint:SetExpand(joint:GetAttribute().Expand)
end