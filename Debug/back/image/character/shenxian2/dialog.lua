

dlgImage = "./image/gui/dialogBGI.png"
dlgImageWidth = 480
dlgImageHeight = 166

NPCImage = "./image/character/shenxian2/Head.png"
NPCImageWidth = 167
NPCImageHeight = 247


----[[
DialogPages = {}

DialogPages.DialogPagesNum = 1
for i = 1, DialogPages.DialogPagesNum, 1 do
	DialogPages[i] = {}
end


DialogPages[1].sentences = {}
DialogPages[1].sentencesNum = 1
for i = 1, DialogPages[1].sentencesNum, 1 do
    DialogPages[1].sentences[i] = {}
end

DialogPages[1].choices = {}
DialogPages[1].choicesNum = 2
for i = 1, DialogPages[1].choicesNum, 1 do
    DialogPages[1].choices[i] = {}
end

DialogPages[1].sentences[1].words = "You need treatment..."
DialogPages[1].sentences[1].align = 0
DialogPages[1].sentences[1].color = -1

--DialogPages[1].sentences[2].words = "welcome"
--DialogPages[1].sentences[2].align = 1
--DialogPages[1].sentences[2].color = -1

DialogPages[1].choices[1].words = "Of Course!"
DialogPages[1].choices[1].color = -3661021
DialogPages[1].choices[1].funcName = "Treatment"

DialogPages[1].choices[2].words = "No.Thanks"
DialogPages[1].choices[2].color = -3661021
DialogPages[1].choices[2].funcName = "CloseDialog"
---3661021

function CloseDialog(dialog)
	local GameGlobalInfo = GetGameGlobalInfo();
	local GUIManager = GameGlobalInfo:GetGUIManager();
	GUIManager:ReleasePanel(dialog);
end

function Treatment(dialog)
	--StartBattle();

	local gameGlobalInfo = GetGameGlobalInfo();
	local player = gameGlobalInfo:GetPlayer();
	local warrior = player:GetWarrior();
	local hpmax = warrior:GetHPMax();
	warrior:SetHP(hpmax);

	local GUIManager = gameGlobalInfo:GetGUIManager();
	GUIManager:ReleasePanel(dialog);

end



function FillDialogText(dialog)

	--[[
	for i = 1, DialogPages.DialogPagesNum, 1 do

		-- read the sentences
		for j = 1, DialogPages[i].sentencesNum, 1 do

			dialog:AddSentence(DialogPages[i].sentences[j].words, DialogPages[i].sentences[j].align, DialogPages[i].sentences[j].color);

		end

		-- read the choices
		for j = 1, DialogPages[i].choicesNum, 1 do

			dialog:AddChoice(DialogPages[i].choices[j].words, DialogPages[i].choices[j].color, DialogPages[i].choices[j].funcName);

		end


	end
	--]]

end


