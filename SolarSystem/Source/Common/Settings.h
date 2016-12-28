#ifndef SETTINGS_42355464392_HEADER
#define SETTINGS_42355464392_HEADER

#include <string>
namespace solar
{
	//Group together default values from different classes
	// -Now mostly from IMGuiViewer and its subclasses
	struct settings
	{
		struct win
		{
			const static std::string title;
			struct size//Size of created Window in pixels
			{
				const static size_t x, y;
			};
			struct pos//Position of created Window in pixels
			{
				const static size_t x, y;
			};
		};

		struct lineTrail
		{
			// Maximum number of points in one lineTrails.
			// GPU Memory footprint:
			// - maxLength*4KB for indices
			// - numUnits* maxLength*8KB for vertices
			const static size_t maxLength;
			// How often get trails updated, counted in viewer's frames
			// Higher values result in more frequent updates and more precise but shorter-lasting trails
			const static size_t resolution;
			// Whether are lineTrails rendered by default for all units
			const static bool enabledByDefault;
		};
		struct circleBuffer
		{
			// Number of vertices to aproximate circle with.
			const static size_t resolution;
			// In normalized screen units = <0,1.0>, 1.0 creates circle that spans across whole horizontal axis
			const static float radius;
		};
	};
}


#endif