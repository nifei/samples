function SetExpand(joint, expand)
	local attr = joint:GetAttribute()
	local prevState = attr.Expand
	joint:GetAttribute().Expand = expand
	local text = joint:GetObject("text")
	text:SetText(expand and "-" or "+")
	if prevState ~= expand then joint:FireExtEvent("Triggered", expand) end
end

function IsExpand(joint)
	return joint:GetAttribute().Expand
end

function OnLButtonUp(joint, x, y)
	joint:SetExpand(not (joint:IsExpand()))
end

function OnInitControl(joint)
	joint:SetExpand(joint:GetAttribute().Expand)
end