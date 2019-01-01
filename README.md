# teleop_twist_monkey

Configurable random twist message publisher for synthetic data generation and test automation.

### Parameters

**linear_min** - minimum linear velocity, default -0.5 m/s

**linear_max** - maximum linear velocity, deafult 0.5 m/s

**angular_min** - minimum angular velocity, default -1.0 rad/s

**angular_max** - maximum angular velocity, deafult 1.0 rad/s

**refresh_duration** - time period to generate new random values

**crossfade_coeff** - smoothing coefficient, default 0.01 ( no filtering when set to 1.0 )

**publish_rate** - frequency of cmd_vel publication, default 50Hz
