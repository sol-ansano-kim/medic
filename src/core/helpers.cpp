#include "medic/helpers.h"
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MStatus.h>

namespace MEDIC
{
    bool IsTemplated(MdNode *node)
    {
        if (node && node->isDag())
        {
            MDagPath dp = node->getPath();
            return IsTemplated(dp);
        }
        else
        {
            return false;
        }
    }

    bool IsTemplated(const MObject &object)
    {
        if (!object.isNull())
        {
            MStatus stat;
            MFnDagNode dn(object, &stat);
            if (stat != MS::kSuccess)
            {
                return false;
            }
            else
            {
                MDagPath dp;
                dn.getPath(dp);
                return IsTemplated(dp);
            }
        }
        else
        {
            return false;
        }
    }

    bool IsTemplated(const MDagPath &path)
    {
        MPlug plug;
        MDagPath dp(path);
        bool overridden = false;

        while (dp.length() > 0)
        {
            MFnDagNode dn(dp);

            if (!overridden)
            {
                plug = dn.findPlug("overrideEnabled", true);
                if (!plug.isNull() && plug.asBool())
                {
                    overridden = true;
                    plug = dn.findPlug("overrideDisplayType", true);
                    if (!plug.isNull() && plug.asShort() == 1)
                    {
                        return true;
                    }
                }
            }

            plug = dn.findPlug("template", true);
            if (!plug.isNull() && plug.asBool())
            {
                return true;
            }

            if (dp.pop() != MStatus::kSuccess)
            {
                break;
            }
        }

        return false;
    }

}