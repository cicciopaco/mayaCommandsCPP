#include "snapNodesCommand.h"


const MString SnapNodesCommand::commandName = "snapNodes";
const char* SnapNodesCommand::commandAuthor = "francescoc";
const char* SnapNodesCommand::commandVersion = "1.0";

SnapNodesCommand::SnapNodesCommand()
{
}

SnapNodesCommand::~SnapNodesCommand()
{
}

MStatus SnapNodesCommand::doIt(const MArgList& argList)
{
	/*
	- extract the flags values (to do)
	- extract the selectes objects as MDagPath/MObject CLASS MEMBERS (add those to the class declaration/header)
	- extract the current SNAPPEE MTransformationMatrix/MMatrix as CLASS MEMBER (add it to the class declaration/header) so to be able to use it in the redoIt()
	- perform sanity check on the selection
	*/
	MStatus status;

	// acquire all the flags 
	MArgDatabase argData(cmdSyntax(), argList, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// get snap alignment options flags
	if (!argData.isFlagSet("-translation") && !argData.isFlagSet("-rotation") && !argData.isFlagSet("-scale"))
	{
		bTranslation = true;
		bRotation = true;
		bScale = true;
	}
	else
	{
		if ( argData.isFlagSet("-translation") ) argData.getFlagArgument("-translation", 0, bTranslation);
		if ( argData.isFlagSet("-rotation") ) argData.getFlagArgument("-rotation", 0, bRotation);
		if ( argData.isFlagSet("-scale") ) argData.getFlagArgument("-scale", 0, bScale);
	}

	// get snapper and snappee transform flags

	if (argData.isFlagSet("-reference") && argData.isFlagSet("-transformNode"))
	{
		// retrive objects from string name
		MString msReference;
		MString msTransform;
		status = argData.getFlagArgument("-reference", 0, msReference);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = argData.getFlagArgument("-transformNode", 0, msTransform);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = sList.add(msReference);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = sList.add(msTransform);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	else
	{
		// cast in MSeslecionList member all the objects selected
		status = argData.getObjects(sList);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}



	// exrtract the selected object in the MDagPath Members
	status = sList.getDagPath(0, snapper);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = sList.getDagPath(1, snappee);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// create the transform function set for the two dagPath
	MFnTransform fnTransform_snapper(snapper);
	MFnTransform fnTransform_snappee(snappee);

	// retrieve transformation info
	status = getTransformations(fnTransform_snapper, vSnapperT, qSnapperR, dSnapperS);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = getTransformations(fnTransform_snappee, vSnappeeT, qSnappeeR, dSnappeeS);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return redoIt();

}

MStatus SnapNodesCommand::redoIt()
{

	// makes controls on the selection list
	/*
	check either the length or the content and discard if they are not transform
	selection must contain kTransform (as MObject.apiTypeStr) or 110 (as MDagPath.apiType())
	*/

	// get the dag path of the two selected elements
	MStatus status;
	MFnTransform fnTransform_snappee(snappee);
	status = setTransformations(fnTransform_snappee, vSnapperT, qSnapperR, dSnapperS);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;

}

MStatus SnapNodesCommand::undoIt()
{
	MGlobal::displayInfo("Executing UNDO on snap command");
	MStatus status;
	MFnTransform fnTransform_snappee(snappee);
	status = setTransformations(fnTransform_snappee, vSnappeeT, qSnappeeR, dSnappeeS);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MSyntax SnapNodesCommand::cmdSyntax()
{
	MSyntax syntax;

	// add flags
	syntax.addFlag("-t", "-translation", MSyntax::kBoolean);
	syntax.addFlag("-r", "-rotation", MSyntax::kBoolean);
	syntax.addFlag("-s", "-scale", MSyntax::kBoolean);
	syntax.addFlag("-rf", "-reference", MSyntax::kString);
	syntax.addFlag("-tn", "-transformNode", MSyntax::kString);

	// allow to operates only on two selected objects
	syntax.useSelectionAsDefault(true);
	syntax.setObjectType(MSyntax::kSelectionList, 2, 2);

	// set flags property
	syntax.enableQuery(isQueriable);
	syntax.enableEdit(isEditable);

	return syntax;
}

MStatus SnapNodesCommand::getTransformations(const MFnTransform& source, MVector& translation, MQuaternion& rotation, double* scale)
{ 
	MStatus status;

	translation = source.getTranslation(MSpace::kWorld, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = source.getRotation(rotation, MSpace::kWorld);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = source.getScale(scale);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;

}

MStatus SnapNodesCommand::setTransformations(MFnTransform& target, const MVector& translation, const MQuaternion& rotation, const double* scale)
{
	MStatus status;

	if (bTranslation)
	{
		status = target.setTranslation(translation, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	if (bRotation)
	{
		status = target.setRotation(rotation, MSpace::kWorld);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	if (bScale)
	{
		status = target.setScale(scale);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	return MS::kSuccess;
}