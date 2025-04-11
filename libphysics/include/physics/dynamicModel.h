#pragma once

#include "physics/platform_core.h"
#include "physics/common.h"

namespace sim::physics
{
    class RigidBody;

	class SIMRACE_DLL DynamicModel
	{
    public:
        /// <summary>
        /// Default constructor
        /// </summary>
        DynamicModel();

        /// <summary>
        /// Computes and returns the hydrodynamics damping force applied to a rigid body
        /// </summary>
        /// <param name="body">Considered rigid body</param>
        /// <param name="rho">water density (use 1026)</param>
        /// <returns>Force / torque</returns>
        common::ForceTorque2D computeDamping(const RigidBody& body, double rho);

        /// <summary>
        /// Computes and returns the added mass force applied to a rigid body
        /// </summary>
        /// <param name="body">Considered rigid body</param>
        /// <returns>Force / torque</returns>
        common::ForceTorque2D computeAddedMass(const RigidBody& body);

        /// <summary>
        /// Computes and returns the force produced by the actuators.
        /// </summary>
        /// <param name="body">Considered rigid body</param>
        /// <param name="RPM">Motor speed (rotation per minute)</param>
        /// <param name="angle">Helm angle in radians</param>
        /// <param name="rho">Water density (use 1026)</param>
        /// <returns>Résulting force / torque</returns>
        common::ForceTorque2D computeActuators(const RigidBody& body, double RPM, double angle, double rho);

        /// <summary>
        /// Resets the dynamic model. Useful to restart the simulation
        /// </summary>
        void reset();

    protected:
        double _alphaFilteredAcc;
        common::Vector2 _filteredAccLin;
        double _filteredAccAng{};
	};
}