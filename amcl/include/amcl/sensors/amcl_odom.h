/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000  Brian Gerkey et al.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
///////////////////////////////////////////////////////////////////////////
//
// Desc: Odometry sensor model for AMCL
// Author: Andrew Howard
// Date: 17 Aug 2003
// CVS: $Id: amcl_odom.h 4135 2007-08-23 19:58:48Z gerkey $
//
///////////////////////////////////////////////////////////////////////////

#ifndef AMCL_ODOM_H
#define AMCL_ODOM_H

#include "amcl_sensor.h"
#include "../pf/pf_pdf.h"

namespace amcl
{

typedef enum
{
  ODOM_MODEL_DIFF,
  ODOM_MODEL_OMNI,
  ODOM_MODEL_DIFF_CORRECTED,
  ODOM_MODEL_OMNI_CORRECTED,
  ODOM_MODEL_OMNI_ROSIE,
  // based on ODOM_MODEL_OMNI_CORRECTED
  ODOM_MODEL_OMNI_SCALED_VARIANCE,
  ODOM_MODEL_OMNI_BIMODAL,
  ODOM_MODEL_OMNI_BIMODAL_SCALED_VARIANCE
} odom_model_t;

// Odometric sensor data
class AMCLOdomData : public AMCLSensorData
{
  // Odometric pose
  public: pf_vector_t pose;

  // Change in odometric pose
  public: pf_vector_t delta;

  // Time elapsed (in seconds) since the last update
  public: double time_elapsed;

  // Quality of the current pose estimate [0.0, 1.0]
  public: double pose_confidence;

  // Set in models with scaled noise based on confidence
  public: double multiplier;
};


// Odometric sensor model
class AMCLOdom : public AMCLSensor
{
  // Default constructor
  public: AMCLOdom();

  public: void SetModelDiff(double alpha1,
                            double alpha2,
                            double alpha3,
                            double alpha4);

  public: void SetModelOmni(double alpha1,
                            double alpha2,
                            double alpha3,
                            double alpha4,
                            double alpha5);

  public: void SetModel( odom_model_t type,
                         double alpha1,
                         double alpha2,
                         double alpha3,
                         double alpha4,
                         double alpha5 = 0,
                         double alpha6 = 0,
                         double max_cov_scale = 5.0,
                         double expected_time_elapsed = 0.1,
                         double peak_mode_delta_pct = 0.2,
                         double noise_floor_scale = 0.1);

  // Update the filter based on the action model.  Returns true if the filter
  // has been updated.
  public: virtual bool UpdateAction(pf_t *pf, AMCLSensorData *data);

  // Current data timestamp
  private: double time;

  // Model type
  private: odom_model_t model_type;

  // Drift parameters
  private: double alpha1, alpha2, alpha3, alpha4, alpha5, alpha6;

  // Max scale factor for ODOM_MODEL_OMNI_SCALED_VARIANCE
  private: double max_cov_scale;

  // expected time between odom updates
  private: double expected_time_elapsed;

  // For bimodal update, how separate should the peaks be (percent of delta)
  private: double peak_mode_delta_pct;

  // Scale noise alphas to produce noise floor and limit over-converged filter
  private: double noise_floor_scale;
};


}

#endif
