{
  "type": "WorldNut",
  "name": "Second Level",
  "position": [0, 0, 40],
  "eulerAngles": [0, 0, 0],
  "scale": [1,1,1],
  "children": [
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
      "name": "Spot Light",
      "position": [0,5,0],
      "eulerAngles": [-1.6,0,0],
      "scale": [1,1,1],
      "diffuseColor": [0 ,1,0],
      "lightType": "spot",
      "spotlightInnerAngle": 0.9,
      "spotlightOuterAngle": 1.0,
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
	  "speed": 5,
      "children": [
		  {
		  "type": "MeshNut",
		  "name": "Enemy Mesh",
		  "mesh": "./Resources/cube.obj",
		  "material": "./Resources/floor.mat",
		  "position": [0, 0, 0],
		  "eulerAngles": [0, 0, 0],
		  "scale": [0.5,2,0.5],
		  "children": []
	  }
	  ]
    }
  ]
}