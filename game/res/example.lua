function Example()
	NewScene(true) -- We can create a new scene by calling NewScene(), parameter describes if a camera should be created or not

	SetCameraPosition(50, 0) -- We can set the camera position

	--To create a Entity or UIElement we can simply call BeginEntity()
	BeginEntity()
		SetPosition(300, 200) -- We can set the localPosition
		SetScale(32, 32) -- We can set scale
		
		--We can set a tag on the entity, further down in the function we can get this instance by BeginExistingEntityByTag()
		SetTag("TaggedEntity")
		
		--We can add component by calling BeginComponent(COMPONENT CLASS NAME) in this case the sprite component is added
		--Note that these components have to be registered in component_register.h, class prefix is automaticly added.
		BeginComponent("Sprite")
			--We can set a property within the component by calling SetProperty(KEY, Values, ....) 
			SetProperty("Texture", "res/terrain_tiles.tga")
			
			--Sprite also has a property to split
			SetProperty("Split", 32, 52) -- First parameter: Amount of Pixels Per Tile, Second parameter: Index
		EndComponent() -- We have to End the component aswell.
		
		--We can child a new entity by enclosing it between the BeginEntity() and EndEntity() parent
		BeginEntity()
			SetPosition(50, 0) -- Parent transformations are included in this transformation, since this is a child
			SetScale(0, 0) -- We dont have to set the scale of the child because scale will be copied from parent
			
			BeginComponent("Sprite")
				SetProperty("Texture", "res/placeholder.tga")
			EndComponent()
		EndEntity()
		
	--We have to call EndEntity to set the current Entity focus to nullptr in C++
	EndEntity()
	
	--To create a UIElement we can simply do:
	BeginEntityUI()
		SetPosition(50, 0)
		SetScale(32,32)
		
		BeginComponent("Sprite")
				SetProperty("Texture", "res/placeholder.tga")
		EndComponent()
		
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