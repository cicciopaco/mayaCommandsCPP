#include "snapNodesCommand.h"

#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, SnapNodesCommand::commandAuthor, SnapNodesCommand::commandVersion);
	status = fnPlugin.registerCommand(SnapNodesCommand::commandName, SnapNodesCommand::creator, SnapNodesCommand::cmdSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj);
	status = fnPlugin.deregisterCommand(SnapNodesCommand::commandName);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return MS::kSuccess;
}