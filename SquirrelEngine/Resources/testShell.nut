{
  "type": "Nut",
  "name": "SceneAssets",
  "children": [
    {
      "type": "Spinner",
      "name": "Handbag",
      "position": [
        0,
        0,
        0
      ],
      "eulerAngles": [
        0,
        0,
        0
      ],
      "scale": [
        1,
        1,
        1
      ],
      "mesh": "./Resources/cube.obj",
      "material": "./Resources/test.mat",
      "children": [
		{
		  "type": "MeshNut",
		  "name": "Child",
		  "position": [
			2,
			2,
			0
		  ],
		  "eulerAngles": [
			0,
			0,
			0
		  ],
		  "scale": [
			1,
			1,
			1
		  ],
		  "mesh": "./Resources/sphere.obj",
		  "material": "./Resources/test.mat",
		  "children": []
		}
	  ]
    },
    {
      "type": "LightNut",
      "name": "WorldLight",
      "position": [
        -5,
        0,
        0
      ],
      "eulerAngles": [
        -1.6,
        0,
        0
      ],
      "scale": [
        1,
        1,
        1
      ],
      "diffuseColor": [
        1,
        0.2,
        0.2
      ],
      "lightType": "point",
      "spotlightInnerAngle": 0.1,
      "spotlightOuterAngle": 0.1,
      "intensity": 1,
      "children": []
    },
    {
      "type": "LightNut",
      "name": "WorldLight2",
      "position": [
        5,
        0,
        0
      ],
      "eulerAngles": [
        -1.6,
        0,
        0
      ],
      "scale": [
        1,
        1,
        1
      ],
      "diffuseColor": [
        0.2,
        0.2,
        1
      ],
      "ambientColor": [
        0.2,
        0.2,
        1
      ],
      "lightType": "point",
      "spotlightInnerAngle": 0.1,
      "spotlightOuterAngle": 0.1,
      "intensity": 1,
      "ambientIntensity": 0.1,
      "linearAttenuation": 0,
      "quadraticAttenuation": 0,
      "children": []
    },
	{
      "type": "FPVCamera",
      "name": "Cam",
      "position": [
        0,
        0,
        -10
      ],
      "eulerAngles": [
        0,
        0,
        0
      ],
      "scale": [
        1,
        1,
        1
      ],
      "isActive": true,
      "children": [
        {
          "type": "MeshNut",
          "name": "campoint",
          "position": [
            0,
            0,
            5
          ],
          "eulerAngles": [
            -1.6,
            0,
            0
          ],
          "scale": [
            0.2,
            0.2,
            0.2
          ],
          "mesh": "./Resources/arrow.obj",
          "material": "./Resources/test.mat",
          "children": []
        }
      ]
    }
  ]
}