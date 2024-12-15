{
      "type": "Bullet",
      "name": "Bullet",
	  "collisionShape": "./Resources/bullet.shape",
	  "density": 1,
	  "elasticity": 0.1,
	  "static": false,
      "position": [0, 0, 0],
      "eulerAngles": [0, 0, 0],
      "scale": [1,1,1],
      "children": [{
		  "type": "MeshNut",
		  "name": "Floor Mesh",
		  "mesh": "./Resources/bullet.obj",
		  "material": "./Resources/floor.mat",
		  "position": [0, 0, 0],
		  "eulerAngles": [0, 3.14, 0],
		  "scale": [1, 1, 1],
		  "children": []
	  }]
    }