# ESP_Garden_Automation

This is just a fun little project for me to get back into the ESP32/8266 world, while also allowing me to play around with and learn Firebase, Flutter, and Git/Github. Any suggestions or additions just create a pull request and edit what is necessary.

Any circuit and CAD designs should also be put on the repo.

## Roadmap

- Microcontroller & Hardware
	- Control the pump to water the plants
	- Get accurate readings from the sensors
		- Read sensor readings
		- Filter sensor readings (use Eigen?)
	- Weather forecasts
		- Download forecasts
		- Adapt watering schedule to the weather forecast
	- Upload data to Firebase
		- Check accurate readings uploaded
  		- Make sure that the format is correct for minimal storage
- Firebase
	- Develop database structure
 	- Check uploads and downloads are working

- Flutter
	- Manual control of the system
	- Download sensor readings off of Firebase
	- Plot graphs on the app
		- Show predictive results on graphs
		- Plot estimated mean

- Git
	- Create a git action for a pull request
	- Create a CI/CD pipeline (if applicable...)