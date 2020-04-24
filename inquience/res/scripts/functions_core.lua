--Basicly fucntion wrappers for Inquience, easy to use and accesibly from within lua
--Written by Jens Heukers
--April 2020

--Request to exit the application
function QuitApplication()
	RequestExit()
end

--Loads the next scene/level safely
function NextLevel()
	NextLevel_Internal()
end