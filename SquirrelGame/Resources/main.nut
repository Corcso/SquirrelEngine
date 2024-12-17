{
  "type": "Nut",
  "name": "Scene",
  "children": [
	{
      "type": "Player",
      "name": "Main Player",
	  "collisionShape": "./Resources/player.shape",
	  "density": 1,
	  "elasticity": 0.1,
	  "static": false,
      "position": [0, 1, 0],
      "eulerAngles": [0, 0, 0],
      "scale": [1,1,1],
      "children": [
		{
		  "type": "CameraNut",
		  "name": "Main Player Camera",
		  "fov": 70,
		  "isActive": true,
		  "position": [0, 0.5, 0],
		  "eulerAngles": [0, 0, 0],
		  "scale": [1,1,1],
		  "children": []
		}
	  ]
    },
	{
      "type": "PhysicsNut",
      "name": "Floor",
	  "collisionShape": "./Resources/floor.shape",
	  "density": 1,
	  "elasticity": 0.1,
	  "static": true,
      "position": [0, 0, 0],
      "eulerAngles": [0, 0, 0],
      "scale": [1,1,1],
      "children": [{
		  "type": "MeshNut",
		  "name": "Floor Mesh",
		  "mesh": "./Resources/cube.obj",
		  "material": "./Resources/floor.mat",
		  "position": [0, 0, 0],
		  "eulerAngles": [0, 0, 0],
		  "scale": [20,1,40],
		  "children": []
	  }]
    },
	{
      "type": "LightNut",
      "name": "World Light",
      "position": [0,5,0],
      "eulerAngles": [0,0,0],
      "scale": [1,1,1],
      "diffuseColor": [1 ,1,1],
      "ambientColor": [1 ,1,1],
      "ambientIntensity": 0.2,
      "lightType": "point",
      "spotlightInnerAngle": 0.0,
      "spotlightOuterAngle": 0.0,
      "intensity": 1,
      "children": []
    },
	{
      "type": "LightNut",
      "name": "Spot Light",
      "position": [0,5,20],
      "eulerAngles": [-1.6,0,0],
      "scale": [1,1,1],
      "diffuseColor": [1 ,0,0],
      "ambientIntensity": 0,
      "lightType": "spot",
      "spotlightInnerAngle": 0.3,
      "spotlightOuterAngle": 0.5,
      "intensity": 1,
      "children": []
    },
	{
      "type": "Enemy",
      "name": "Enemy Number 1",
	  "collisionShape": "./Resources/player.shape",
	  "density": 1,
	  "elasticity": 0.1,
	  "static": false,
      "position": [5, 1, 5],
      "eulerAngles": [0, 0, 0],
      "scale": [1,1,1],
	  "speed": 2,
      "children": [
		  {
		  "type": "MeshNut",
		  "name": "Enemy Mesh",
		  "mesh": "./Resources/cube.obj",
		  "material": "./Resources/enemy.mat",
		  "position": [0, 0, 0],
		  "eulerAngles": [0, 0, 0],
		  "scale": [0.5,2,0.5],
		  "children": []
	  }
	  ]
    },
	{
		  "type": "AirlockSceneLoading",
		  "name": "SceneLoadTrigger",
		  "children": []
	  }
  ]
}