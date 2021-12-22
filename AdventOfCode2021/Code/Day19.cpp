#include "Day19.h"
#include "Utilities/Utilities.h"
#include "Utilities/RobinHood.h"

void Day19::ReadInput(std::ifstream& stream)
{
	Input.reserve(40);

	enum class ParserState : uint8_t { Scanner, Values };

	char buffer[8192];
	ParserState state = ParserState::Scanner;
	int values[3]{};
	int i = 0;
	bool negative = false;
	bool hasValue = false;

	std::vector<Vec3D>* scanner = &Input.emplace_back();

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (state == ParserState::Scanner)
			{
				if (*c == '\n')
					state = ParserState::Values;
			}
			else
			{
				if (*c == '\n')
				{
					if (hasValue)
					{
						if (negative)
							values[i] *= -1;
						scanner->emplace_back(values[0], values[1], values[2]);

						values[0] = 0;
						values[1] = 0;
						values[2] = 0;
						negative = false;
						hasValue = false;
						i = 0;
					}
					else
					{
						state = ParserState::Scanner;
						scanner = &Input.emplace_back();
					}
				}
				else if (*c == ',')
				{
					if (negative)
						values[i] *= -1;
					negative = false;
					i++;
				}
				else if (*c == '-')
					negative = true;
				else if (*c >= '0' && *c <= '9')
				{
					values[i] *= 10;
					values[i] += *c - '0';
					hasValue = true;
				}
			}

			c++;
			bytes--;
		}
	}

	if (hasValue)
	{
		if (negative)
			values[i] *= -1;
		scanner->emplace_back(values[0], values[1], values[2]);
	}
}

static const Matrix RotationMatrices[24] = {
		{0, 0, -1, 0, -1, 0, -1, 0, 0},
		{0, 0, -1, 1, 0, 0, 0, -1, 0},
		{0, 0, -1, 0, 1, 0, 1, 0, 0},
		{0, 0, -1, -1, 0, 0, 0, 1, 0},
		{0, -1, 0, -1, 0, 0, 0, 0, -1},
		{0, -1, 0, 0, 0, 1, -1, 0, 0},
		{0, -1, 0, 1, 0, 0, 0, 0, 1},
		{0, -1, 0, 0, 0, -1, 1, 0, 0},
		{-1, 0, 0, 0, 0, -1, 0, -1, 0},
		{-1, 0, 0, 0, 1, 0, 0, 0, -1},
		{-1, 0, 0, 0, 0, 1, 0, 1, 0},
		{-1, 0, 0, 0, -1, 0, 0, 0, 1},
		{0, 0, 1, 0, -1, 0, 1, 0, 0},
		{0, 0, 1, -1, 0, 0, 0, -1, 0},
		{0, 0, 1, 0, 1, 0, -1, 0, 0},
		{0, 0, 1, 1, 0, 0, 0, 1, 0},
		{0, 1, 0, -1, 0, 0, 0, 0, 1},
		{0, 1, 0, 0, 0, -1, -1, 0, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, -1},
		{0, 1, 0, 0, 0, 1, 1, 0, 0},
		{1, 0, 0, 0, 0, -1, 0, 1, 0},
		{1, 0, 0, 0, -1, 0, 0, 0, -1},
		{1, 0, 0, 0, 0, 1, 0, -1, 0},
		{1, 0, 0, 0, 1, 0, 0, 0, 1},
};

typedef std::tuple<Vec3D, Vec3D> Points;
typedef std::tuple<int, Points> FingerprintToPointsPair;

struct ScannerData
{
	Vec3D Position;
	std::vector<Vec3D> Beacons;
	std::vector<FingerprintToPointsPair> FingerpintsToPointsInScannerSpace;
};

inline void GenerateFingerprintsForBeacons(const std::vector<Vec3D>& scannerBeacons, robin_hood::unordered_map<int, Points>& fingerprintsToPoints)
{
	for (int a = 0; a < scannerBeacons.size() - 1; a++)
		for (int b = a + 1; b < scannerBeacons.size(); b++)
		{
			const auto& aa = scannerBeacons[a];
			const auto& bb = scannerBeacons[b];
			auto fingerprint = Vec3D::DistanceSquared(aa, bb);
			fingerprintsToPoints[fingerprint] = { aa, bb };
		}
}

inline void GenerateFingerprintsForBeacons(const std::vector<Vec3D>& scannerBeacons, std::vector<FingerprintToPointsPair>& fingerprintsToPoints)
{
	for (int a = 0; a < scannerBeacons.size() - 1; a++)
		for (int b = a + 1; b < scannerBeacons.size(); b++)
		{
			const auto& aa = scannerBeacons[a];
			const auto& bb = scannerBeacons[b];
			auto fingerprint = Vec3D::DistanceSquared(aa, bb);
			fingerprintsToPoints.push_back({ fingerprint, { aa, bb } });
		}
}

inline bool TryTransformBeaconsToZeroSpace(const robin_hood::unordered_set<Vec3D>& beaconsInZeroSpace, const robin_hood::unordered_map<int, Points>& fingerprintsInZeroSpace, ScannerData& scannerData)
{
	// Find all pairs that match with existing fingerprints in ZeroSpace
	std::vector<const FingerprintToPointsPair*> marchingPairs;
	for (const auto& pair : scannerData.FingerpintsToPointsInScannerSpace)
	{
		const auto& [fingerprint, points] = pair;
		if (fingerprintsInZeroSpace.contains(fingerprint))
			marchingPairs.push_back(&pair);
	}

	// If we find less than 66 matching pairs (12 matching beacons), we don't have a match
	if (marchingPairs.size() < 66)
		return false;

	// Loop each matching pair, looking for a rotation that works
	for (auto pair : marchingPairs)
	{
		const auto& [fingerprint, newPoints] = *pair;
		const auto& zeroSpacePoints = fingerprintsInZeroSpace.at(fingerprint);

		const auto& [aa1, aa2] = zeroSpacePoints;
		const auto& [bb1, bb2] = newPoints;

		// Workaround for issue where lamdas can't capture exploded tuples. Fixed in C++20, but that requires VS2022.
		const Vec3D& aa1_ = aa1;
		const Vec3D& aa2_ = aa2;
		const Vec3D& bb1_ = bb1;
		const Vec3D& bb2_ = bb2;
		//

		const auto* workingRotation = std::find_if(
			std::begin(RotationMatrices),
			std::end(RotationMatrices),
			[&](const Matrix& matrix) { return (aa1_ - (bb1_ * matrix) == aa2_ - (bb2_ * matrix)); }
		);

		// If we've found a rotation that works for this point, check that it works for 12 points before we can be sure it actually works
		if (workingRotation)
		{
			const Matrix& matrix = *workingRotation;
			auto offset = aa1 - (bb1 * matrix);

			int matches = 0;
			for (const auto& beacon : scannerData.Beacons)
			{
				Vec3D transformed = (beacon * matrix) + offset;
				if (beaconsInZeroSpace.contains(transformed))
					matches += 1;

				// If we have a match, transform the beacon positions to ZeroSpace
				if (matches >= 12)
				{
					scannerData.Position = offset;
					for (auto& beacon : scannerData.Beacons)
						beacon = (beacon * matrix) + offset;

					return true;
				}
			}
		}
	}

	return false;
}

int Run(const std::vector<std::vector<Vec3D>>& input, bool isPart2)
{
	// We already know the position of all beacons from the first scanner
	robin_hood::unordered_set<Vec3D> beaconsInZeroSpace(input[0].begin(), input[0].end());

	// So we initially generate fingerprints for all beacons from the first scanner
	robin_hood::unordered_map<int, Points> fingerprintsInZeroSpace;
	GenerateFingerprintsForBeacons(input[0], fingerprintsInZeroSpace);

	// Add first scanner to list of scanners. We skip adding any data here, other than position, as we wont use it.
	std::vector<ScannerData> scanners;
	scanners.reserve(input.size());
	scanners.emplace_back().Position = Vec3D();

	// Generate fingerprints for all other scanners and add them to remaining list
	std::vector<ScannerData*> remainingScanners1;
	std::vector<ScannerData*> remainingScanners2;
	for (auto it = input.begin() + 1; it != input.end(); it++)
	{
		ScannerData& data = scanners.emplace_back();
		data.Beacons = *it;
		GenerateFingerprintsForBeacons(*it, data.FingerpintsToPointsInScannerSpace);

		remainingScanners1.push_back(&data);
	}

	// Match all remaining scanners to beacons we already know about in ZeroSpace
	std::vector<ScannerData*>* currentRemainingScannerList = &remainingScanners1;
	std::vector<ScannerData*>* otherRemainingScannerList = &remainingScanners2;
	while (currentRemainingScannerList->size() > 0)
	{
		for (ScannerData* scanner : *currentRemainingScannerList)
		{
			if (TryTransformBeaconsToZeroSpace(beaconsInZeroSpace, fingerprintsInZeroSpace, *scanner))
			{
				// Re-generate the fingerprints for the transformed beacons and add them to the list of ZeroSpace fingerprints
				GenerateFingerprintsForBeacons(scanner->Beacons, fingerprintsInZeroSpace);

				// Add list of transformed beacons to ZeroSpace beacons
				beaconsInZeroSpace.insert(scanner->Beacons.begin(), scanner->Beacons.end());
			}
			else
			{
				// If we couldn't find a match yet, simply put it at the end of the list and try again later
				otherRemainingScannerList->push_back(scanner);
			}
		}

		currentRemainingScannerList->clear();
		std::swap(currentRemainingScannerList, otherRemainingScannerList);
	}

	if (isPart2)
	{
		int maxDistance = 0;
		for (int a = 0; a < scanners.size() - 1; a++)
			for (int b = a + 1; b < scanners.size(); b++)
			{
				Vec3D diff = scanners[a].Position - scanners[b].Position;
				int dist = std::abs(diff.X) + std::abs(diff.Y) + std::abs(diff.Z);
				if (dist > maxDistance)
					maxDistance = dist;
			}

		return maxDistance;
	}
	else
		return (int)beaconsInZeroSpace.size();
}

int Day19::RunA()
{
	return Run(Input, false);
}

int Day19::RunB()
{
	return Run(Input, true);
}