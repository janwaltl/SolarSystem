#include "SolarParser.h"
#include "Source/Units/PhysicsUnits.h"
#include "SerializeUnit.h"
#include "Source/Common/Exception.h"

#include <iostream>

namespace solar
{
	SolarParser::SolarParser(const std::string & saveFile) :
		saveFile(saveFile)
	{}
	SimData SolarParser::Load()
	{
		SimData data;
		//https://nssdc.gsfc.nasa.gov/planetary/factsheet/
		data.SetPhysUnits(PhysUnits::kilogram, PhysUnits::meter, PhysUnits::second);
		//Sun
		data->push_back(Unit(Vec3d(0.0, 0.0), Vec3d(0.0, 0.0), 1.98843e30, "Sun", Vec4d(1.0, 1.0, 0.0, 1.0), 695.7e6));
		//Planets
		data->push_back(Unit(Vec3d(57.909e9, 0.0), Vec3d(0.0, 47362.0), 3.3011e23, "Mercury", Vec4d(0.2, 0.3, 0.2, 1.0), 2394e3));
		data->push_back(Unit(Vec3d(108.208e9, 0.0), Vec3d(0.0, 35020.0), 4.8675e24, "Venus", Vec4d(0.5, 0.2, 0.2, 1.0), 6052e3));
		data->push_back(Unit(Vec3d(149.6e9, 0.0), Vec3d(0.0, 29800.0), 5.9736e24, "Earth", Vec4d(0.17, 0.63, 0.83, 1.0), 6378e3));
		data->push_back(Unit(Vec3d(225.6e9, 0.0), Vec3d(0.0, 24100.0), 6.4185e23, "Mars", Vec4d(1.0, 0.46, 0.0, 1.0), 3396e3));
		data->push_back(Unit(Vec3d(778.5e9, 0.0), Vec3d(0.0, 13050.0), 1.8990e27, "Jupiter", Vec4d(0.86, 0.76, 0.57, 1.0), 71.492e6));
		data->push_back(Unit(Vec3d(1429e9, 0.0), Vec3d(0.0, 9638.0), 5.6846e26, "Saturn", Vec4d(1.0, 0.92, 0.78, 1.0), 60.268e6));
		data->push_back(Unit(Vec3d(2870e9, 0.0), Vec3d(0.0, 6795.0), 8.6832e25, "Uran", Vec4d(0.71, 0.98, 0.96, 1.0), 25.559e6));
		data->push_back(Unit(Vec3d(4498e9, 0.0), Vec3d(0.0, 5432.0), 1.0243e26, "Neptun", Vec4d(0.0, 0.3, 0.77, 1.0), 24.764e6));
		//Dwarf planets
		data->push_back(Unit(Vec3d(7375.93e9, 0.0), Vec3d(0.0, 3710.0), 1.303e22, "Pluto", Vec4d(1.0, 1.0, 1.0, 1.0),1185e3));

		//Moons
		data->push_back(Unit(Vec3d(150.0e9, 0.0), Vec3d(0.0, 30822.0), 7.3476e22, "Moon", Vec4d(0.2, 0.2, 0.2, 1.0), 1737e3));

		data->push_back(Unit(Vec3d(225.609376e9, 0.0), Vec3d(0.0, 26238.0), 1.0659e16, "Phobos", Vec4d(0.5, 0.46, 0.3, 1.0), 11e3));
		data->push_back(Unit(Vec3d(225.623463e9, 0.0), Vec3d(0.0, 25451.0), 1.4762e15, "Deimos", Vec4d(0.2, 0.8, 0.7, 1.0), 6e3));

		data->push_back(Unit(Vec3d(778.9217e9, 0.0), Vec3d(0.0, 30350.0), 8.931938e22, "Io", Vec4d(1.0, 0.2, 0.1, 1.0), 1821e3));
		data->push_back(Unit(Vec3d(779.1709e9, 0.0), Vec3d(0.0, 26790.0), 4.799844e22, "Europa", Vec4d(1.0, 0.8, 0.7, 1.0), 1560e3));
		data->push_back(Unit(Vec3d(779.5704e9, 0.0), Vec3d(0.0, 23930.0), 1.4819e23, "Ganymede", Vec4d(0.6, 0.76, 0.95, 1.0), 2634e3));
		data->push_back(Unit(Vec3d(780.3827e9, 0.0), Vec3d(0.0, 21254.0), 1.075938e23, "Callisto", Vec4d(0.4, 0.1, 0.92, 1.0), 2410e3));


		data.ConvertTo(PhysUnits::sun, PhysUnits::AU, PhysUnits::year);
		data.Normalize();
		return data;
	}
	void SolarParser::Save(const SimData & data)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!saveFile.empty())
		{

			std::ofstream outputF(saveFile, std::ios::trunc | std::ios::out);
			if (!outputF.is_open())
				throw Exception("Output file \'" + saveFile + "\' could not be created/opened.");

			std::cout << "Saving simulated data to file: " << saveFile << '\n';
			for (const auto& unit : data.Get())
				PrintUnit(outputF, unit, data.GetPhysUnits(), PhysUnits {PhysUnits::kilogram,PhysUnits::second,PhysUnits::meter});
			std::cout << data->size() << " units saved.\n";
		}
	}
}
