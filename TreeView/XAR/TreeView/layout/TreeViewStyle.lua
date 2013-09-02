function RegisterGlobal()
	XLSetGlobal("GetTreeStyle", GetTreeStyle)
end

function GetTreeStyle(tree)
	local attr = tree:GetAttribute()
	local styleName = attr.style_name
	if styleName == "WindowsStyle" or styleName==nil then 
		local style=GetWindowsStyle()
		style.icon_size = {width=attr.icon_width, height =attr.icon_height}
		style.horizontal_indent = attr.horizontal_indent
		style.node_size = {width=attr.node_width, height = attr.node_height}
		style.vertical_padding = attr.vertical_padding
		return style
	end
end

function GetWindowsStyle()
	local style={}
	-- in fatherobjsize, children sub tree layout size list
	-- out father obj pos, children sub tree layout pos list
	style.GetChildrenPosList = 
		function (fsize, sizelist)
			local poslist = {}
			local indent = style.horizontal_indent and style.horizontal_indent or 30-- todo Style attribute
			local left = indent
			local padding = style.vertical_padding and style.vertical_padding or 20
			local top = padding + fsize.height
			for k,v in pairs(sizelist) do
				poslist[k] = {left=left, top=top}
				top = top + sizelist[k].height+padding
			end
			return {left=0,top=0},poslist
		end
	style.GetLinesAndJointPosList = 
		function (frect, rectlist)
			local iconSize = style.icon_size and style.icon_size or {width=20, height = 20} -- todo default
			local linePosList = {}
			local jointPosList = {}
			local left = iconSize.width/2
			local bottom = frect.top+frect.height
			for k, rect in pairs(rectlist) do
				linePosList[k] = {left = iconSize.width/2 , top = rect.top +rect.height/2, right = rect.left, bottom = rect.top +rect.height/2}
				bottom = bottom < rect.top + rect.height/2 and rect.top + rect.height/2 or bottom
			end
			linePosList["main_line"] = {left = left, top = frect.top+frect.height, right = left, bottom = bottom}
			return linePosList, jointPosList
		end
	return style
end

function GetTopMidFatherStyle()
	local style={}
	-- in fatherobjsize, children sub tree layout size list
	-- out father obj pos, children sub tree layout pos list
	style.GetChildrenPosList = 
		function (fsize, sizelist)
			local poslist = {}
			local left = 0
			local top = fsize.height
			local height = 0
			local width = 0
			local padding = 10
			for k,v in pairs(sizelist) do
				local size = sizelist[k]
				poslist[k] ={left=left, top=top}
				left = left + size.width + padding
				width = width + v.width + padding
			end
			width = width-padding
			return {left=width/2-fsize.width/2, top=0}, poslist
		end
	return style
end

-- client method
-- in size+sizelist
-- out pos + poslist
-- 位置是相对fatherObj左上角的
function GetLeftMidFatherStyle()
	local style={}
	-- in fatherobjsize, children sub tree layout size list
	-- out father obj pos, children sub tree layout pos list
	style.GetChildrenPosList = 
		function (fsize, sizelist)
			local poslist = {}
			local left = fatherObjSize.width + 10
			local height = 0
			for k,v in pairs(sizelist) do
				height = height + v.height
			end
			local top = 0
			for k,v in pairs(sizelist) do
				local size = sizelist[k]
				poslist[k] ={left=left, top=top}
				top = top+size.height
			end
			return {left=0, top=top/2-fatherObjSize.height/2}, poslist
		end
	return style
end