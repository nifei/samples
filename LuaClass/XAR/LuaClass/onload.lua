local path = __document
local index = string.find(path, "/[^/]*$")
local rootDir = string.sub(path,1,index)

local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
local frameHostWndTemplate = templateManager:GetTemplate("MyBolt.Wnd","HostWndTemplate")
local frameHostWnd = frameHostWndTemplate:CreateInstance("MainFrame")

if frameHostWnd then
	local objectTreeTemplate = templateManager:GetTemplate("MyBolt.Tree", "ObjectTreeTemplate")
	if objectTreeTemplate then 
		local uiObjectTree=objectTreeTemplate:CreateInstance("MainObjectTree")
		frameHostWnd:BindUIObjectTree(uiObjectTree)
		frameHostWnd:Create()
	end
end