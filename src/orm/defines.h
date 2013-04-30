#ifndef MELAMPIG_DEFINES_H
#define MELAMPIG_DEFINES_H

#include <QList>
#include <QHash>

namespace Melampig
{

typedef QList<QVariant> TQueryParam;
typedef QHash<QString, QVariant> TQueryMap;

enum Expression
{
	Equal = 1,             // just equal: id = 10
	Not = 2,               // just not equal: id != 10
	Greater = 4,           // used with Equal: id >= 10
	Less = 8,              // used with Equal: id <= 10
	Interval = 16,         // search in range: ( 1 < id ) and ( id < 10 )
	Segment = 32,          // search in range with end-points: ( 1 <= id ) and ( id <= 10 )
	InSet = 64,            // find in set: id IN ( 1,2,3 )
	Like = 128,            // pattern matching
	Case = 256,            // case insensitive tree as ILIKE or lower()

	Field = 512,

	// used with user defined types
	Intersect = 1024,
	Contains = 2048,
	Within = 4096,

	And = 8192,
	Or = 16384,
	Is = 32768,

	// user defined types
	// Treat upper bits for bottom types
	// Starts with: 2^16

	// Access list for object
	// Intersect: &&
	// Contains: &>
	// Whithin: &<
	RowAcl = 65536,

	// Queries for data organized as a tree-like structures
	// Intersect: @>
	// Contains: <@
	LTree = 131072,

	// one-dimensional array of int4's
	// Intersect: &&
	// Contains: @
	// Within: ~
	IntArray = 262144,

	// Queries for data organized as a hash-like structures
	// Intersect: @>
	// Contains: <@
	HStore = 524288
	};

enum ObjectType
{
	ONull = 0,
	OObject = 1,
	OTree = 2,
	OLink = 3,
	OAction = 4,
	OAddress = 5,
	OAddressType = 6,
	OAge = 7,
	OCircle = 8,
	OClassify = 9,
	OCoach = 10,
	OCompetition = 11,
	OCompetitionMember = 12,
	OCompetitionReferee = 13,
	OCompetitionStyle = 14,
	OCompetitionTeam = 15,
	OCompetitionWeight = 16,
	OCompetitionWrestler = 17,
	OCRound = 18,
	OCTour = 19,
	OCompType = 20,
	OFight = 21,
	OFightLog = 22,
	OFRound = 23,
	OGender = 24,
	OGeo = 25,
	OMat = 26,
	OMatch = 27,
	OMember = 28,
	OPosition = 29,
	ORang = 30,
	OReferee = 31,
	ORole = 32,
	OStyle = 33,
	OTeam = 34,
	OWeight = 35,
	OWrestler = 36,
	OMemberAddress = 37,
	OWrestlerAddress = 38,
	OCoachAddress = 39,
	ORefereeAddress = 40,
	OCGroup = 41
};

enum ModelType
{
	aList = 1,
	aTable = 2,
	aTree = 3
};

}

#endif // MELAMPIG_DEFINES_H
