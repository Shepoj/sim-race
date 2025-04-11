#pragma once
#include "physics/platform_core.h"
#include "physics/common.h"

#include <memory>
#include <vector>

namespace sim::physics
{
struct State
{
	/// Linear acceleration x / y (m/s-2)
	common::Vector2 acc_body {};
	/// Angular acceleration around the vertical axis (rad/s-2)
	double rdot{};
	
	/// Linear velocity x / y (m/s)
	common::Vector2 velocity_body {};
	/// Angular velocity around the vertical axis (rad/s)
	double r {};

	/// Position x / y (m)
	common::Vector2 position {};
	/// Rotation around the vertical axis (rad)
	double yaw {};

private:
	/// Linear acceleration x / y
	common::Vector2 acc;
};

/// <summary>
/// Standard config struct for any vehicle
/// </summary>
struct ModelConfig
{
	double mass; // kg
	double Iz;   // Inertia on Z
	double referenceArea; // Reference area of the boat
	double CX;
	double CX2;
	double CY2;
	double CYuv; 
	double CYur;
	double CN;
	double CNvr;
	double propellerDiameter; // m
	double propellerEfficiency; // scalar efficiency
	common::Vector2 propellerAppliPoint_body;
	double Mx; // Added mass on X
	double My; // Added mass on Y
	double MIz;// Added inertia on Z
};

class PhysicsEngine;

/// <summary>
/// Represents the physics part of a vehicle
/// </summary>
class SIMRACE_DLL RigidBody
{
public:
	/// <summary>
	/// Constructs a RigidBody
	/// </summary>
	/// <param name="config">Dynamic model to use</param>
	RigidBody(const ModelConfig& config);

	/// <summary>
	/// Adds a force / torque to the rigid body 
	/// </summary>
	/// <param name="force"></param>
	void addForce(common::ForceTorque2D& force);

	/// <summary>
	/// Returns the current force applied to the rigid body
	/// </summary>
	/// <returns></returns>
	const common::ForceTorque2D& getForce() const;
	
	/// <summary>
	/// Returns the current body's state
	/// </summary>
	/// <returns>A reference to the body's state</returns>
	const State& getState() const;
	/// <summary>
	/// Retrieves the boat dynamic model
	/// </summary>
	/// <returns>A reference to the dynamic model</returns>
	const ModelConfig& getModel() const;

	/// <summary>
	/// Sets the current position of the RigidBody
	/// </summary>
	/// <param name="x">Current position on the X axis (m)</param>
	/// <param name="y">Current position on the Y axis (m)</param>
	void setPosition(double x, double y);

	/// <summary>
	/// Sets the current rotation of the rigid body
	/// <param name="rotRad">Rotation angle to apply (rad)</param>
	/// </summary>
	void setRotation(double rotRad);

	/// <summary>
	/// Sets the current body's velocity
	/// </summary>
	/// <param name="vx">velocity on X axis</param>
	/// <param name="vy">velocity on Y axis</param>
	void setVelocity_body(double vx, double vy);

	/// <summary>
	/// Sets the angular velocity
	/// </summary>
	/// <param name="r">Angular velocity (rad)</param>
	void setAngularVelocity(double r);

	/// <summary>
	/// Converts a force or whatever direction vector from the local body coordinates to the world coordinates
	/// <param name="bodyFrame">Rotation angle to apply (rad)</param>
	/// </summary>
	sim::common::Vector2 toWorld(const sim::common::Vector2& bodyFrame);

	/// <summary>
	/// Converts a force or whatever direction vector from the world frame to the local body coordinates
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	sim::common::Vector2 toBody(const sim::common::Vector2& worldFrame);

	friend class PhysicsEngine;

protected:
	/// <summary>
	/// Resets the force applied
	/// </summary>
	void reset();

protected:
	common::ForceTorque2D _force;
	State _state;
	ModelConfig _model;

};

/// <summary>
/// Class responsible for the physics behaviour of the vehicles. 
/// </summary>
class SIMRACE_DLL PhysicsEngine
{
public:
	/// <summary>
	/// Advances the simulation by a stepsize
	/// <param name="dt">Stepsize in seconds</param>
	/// </summary>
	void step(double dt);

	/// <summary>
	/// Creates a new RigidBody with default values
	/// <returns>A pointer to the created rigid body. Allocation and deallocation 
	/// of the pointer is the responsibility the physics engine</returns>
	/// </summary>
	RigidBody* createGenericBoat();

	/// <summary>
	/// Creates a new RigidBody with model values
	/// <param name="config">Model parameters</param>
	/// <returns>A pointer to the created rigid body. Allocation and deallocation 
	/// of the pointer is the responsibility the physics engine</returns>
	/// </summary>
	RigidBody* createBoat(const ModelConfig& config);

	/// <summary>
	/// Removes an existing RigidBody from the physics engine
	/// <returns>true if the body was removed, false otherwise</returns>
	/// </summary>
	bool removeBoat(RigidBody* body);

	// Prevent copy
	PhysicsEngine() = default;
	PhysicsEngine(PhysicsEngine&&) = default;
	PhysicsEngine(PhysicsEngine&) = delete;
	PhysicsEngine& operator = (const PhysicsEngine&) = delete;

protected:
	std::vector<std::unique_ptr<RigidBody>> _rigidbodies;

	//double _dt;
};

}