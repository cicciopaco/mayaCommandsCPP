
#ifndef SNAPNODESCOMMAND_H
#define SNAPNODESCOMMAND_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MString.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>

#include <maya/MGlobal.h>


class SnapNodesCommand : public MPxCommand
{
public:
	SnapNodesCommand();
	~SnapNodesCommand();

	virtual MStatus doIt(const MArgList& argList);
	virtual MStatus redoIt();
	virtual MStatus undoIt();

	virtual bool isUndoable() const { return true; }

	static void *creator() { return new SnapNodesCommand; }
	static MSyntax cmdSyntax();

	static const MString commandName;
	static const char* commandAuthor;
	static const char* commandVersion;


	static const bool isQueriable = false;
	static const bool isEditable = false;

private:
	MSelectionList sList;
	MDagPath snapper, snappee;

	MVector vSnapperT, vSnappeeT;
	MQuaternion qSnapperR, qSnappeeR;
	double dSnapperS[3], dSnappeeS[3];

	MStatus getTransformations(const MFnTransform& source, MVector& translation, MQuaternion& rotation, double* scale);
	MStatus setTransformations(MFnTransform& target, const MVector& translation, const MQuaternion& rotation, const double* scale);


	// flags
	bool bTranslation{ false }, bRotation{ false }, bScale{ false };



};

#endif // !SNAPNODESCOMMAND_H

