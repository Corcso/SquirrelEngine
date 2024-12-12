{
	"type": "Nut",
	"name": "SceneAssets",
	"children": [
	{
		"type": "MeshNut",
		"name": "Handbag",
		"position": [0, 0, 0],
		"eulerAngles": [0, 0, 0.8],
		"scale": [1, 1, 1],
		"mesh": "./Resources/HQsphere.obj",
		"material": "./Resources/test.mat",
		"children": []
	},
	{
		"type": "FPVCamera",
		"name": "Cam",
		"position": [0, 0, -10],
		"eulerAngles": [0, 0, 0],
		"scale": [1, 1, 1],
		
		"isActive": true,
		"children": []
	},
	{
		"type": "LightNut",
		"name": "WorldLight",
		"position": [-5, 0, 0],
		"eulerAngles": [-1.6, 0, 0],
		"scale": [1, 1, 1],
		"diffuseColor": [1, 0.2, 0.2],
		"lightType": "point",
		"spotlightInnerAngle": 0.1,
		"spotlightOuterAngle": 0.1,
		"intensity": 1,
		"children": []
	},
	{
		"type": "MeshNut",
		"name": "lightShower",
		"position": [0, 5, 0],
		"eulerAngles": [-1.6, 0, 0],
		"scale": [0.2, 0.2, 0.2],
		"mesh": "./Resources/arrow.obj",
		"material": "./Resources/test.mat",
		"children": []
	},
	{
		"type": "LightNut",
		"name": "WorldLight2",
		"position": [5, 0, 0],
		"eulerAngles": [-1.6, 0, 0],
		"scale": [1, 1, 1],
		"diffuseColor": [0.2, 0.2, 1],
		"lightType": "point",
		"spotlightInnerAngle": 0.1,
		"spotlightOuterAngle": 0.1,
		"intensity": 1,
		"children": []
	}
	]
}