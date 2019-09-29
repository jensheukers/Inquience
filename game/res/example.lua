function Example()
	--To create a Entity or UIElement we can simply call BeginEntity(TEXTUREPATH, (Optional)"UI")
	BeginEntity("res/terrain_tiles.tga")
		SetPosition(300, 200) -- We can set the localPosition
		SetScale(32, 32) -- We can set scale
		
		--We can set a tag on the entity, further down in the function we can get this instance by BeginExistingEntityByTag()
		SetTag("TaggedEntity")
		
		--We can also split the texture, this will update the UV coordinates in Sprite Component
		Split(32, 52) -- First parameter: Amount of Pixels Per Tile, Second parameter: Index
	
		--We can child a new entity by enclosing it between the BeginEntity() and EndEntity() parent
		BeginEntity("res/terrain_tiles.tga")
			SetPosition(50, 0) -- Parent transformations are included in this transformation, since this is a child
			SetScale(0, 0) -- We dont have to set the scale of the child because scale will be copied from parent
			
			--We can also split the child
			Split(32, 53)
		EndEntity()
		
	--We have to call EndEntity to set the current Entity focus to nullptr in C++
	EndEntity()
	
	--To create a UIElement we can simply do:
	BeginEntity("res/placeholder.tga", "UI")
		SetPosition(50, 0)
		SetScale(32,32)
		
		--UIElement have something special, these are 3 methods that allow for functions to be called back in lua
		OnEnter("res/example.lua", "Event", "You entered my element")
		OnStay("res/example.lua", "Event", "You are staying on my element")
		OnLeave("res/example.lua", "Event", "You have left my entity")
	--End the entity
	EndEntity()
	
	--We can get the entity we created earlier and change its position
	BeginExistingEntityByTag("TaggedEntity")
		SetPosition(700, 50)
	EndEntity()
	
	--We can play sounds in the engine by calling
	--PlaySound("res/example.ogg", 1, 1) -- Last 2 parameters are Pitch and Gain
end

function Event(message)
	--We can Log messages to the console by calling Log()
	Log(message)
end