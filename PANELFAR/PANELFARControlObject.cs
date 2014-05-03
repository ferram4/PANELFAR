using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using KSP;

namespace panelfar
{
    [KSPAddon(KSPAddon.Startup.Flight, false)]
    public class PANELFARControlObject : MonoBehaviour
    {
        private PANELFARPartLocalMesh debugMesh;
        private void CreateMeshSimulationMeshVessel(Vessel v)
        {
            if (v.isActiveVessel)
                debugMesh = PANELFARPartLocalMeshGenerator.GenerateFromPart(v.rootPart);
        }

        private void LateUpdate()
        {
            if (debugMesh != null)
                debugMesh.DebugDrawMesh();
            else
                CreateMeshSimulationMeshVessel(FlightGlobals.ActiveVessel);
        }
    }
}
