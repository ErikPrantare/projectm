/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed i the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#include <sstream>
#include "Common.hpp"
#include "fatal.h"

#include "CustomShape.hpp"
#include "Eval.hpp"
#include "Expr.hpp"
#include "InitCond.hpp"
#include "Param.hpp"
#include "PerFrameEqn.hpp"
#include "Preset.hpp"
#include <map>
#include "ParamUtils.hpp"
#include "InitCondUtils.hpp"
#include "wipemalloc.h"


CustomShape::CustomShape()
    : CustomShape(0)
{
}

CustomShape::CustomShape(int _id)
    : Shape()
{
    this->id = _id;
    this->per_frame_count = 0;

    /* Start: Load custom shape parameters */
    ParamUtils::insert(Param::new_param_float(
        "r", P_FLAG_NONE, &this->r, nullptr, 1.0, 0.0, 0.5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "g", P_FLAG_NONE, &this->g, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "b", P_FLAG_NONE, &this->b, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "a", P_FLAG_NONE, &this->a, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "border_r", P_FLAG_NONE, &this->border_r, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "border_g", P_FLAG_NONE, &this->border_g, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "border_b", P_FLAG_NONE, &this->border_b, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "border_a", P_FLAG_NONE, &this->border_a, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "r2", P_FLAG_NONE, &this->r2, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "g2", P_FLAG_NONE, &this->g2, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "b2", P_FLAG_NONE, &this->b2, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "a2", P_FLAG_NONE, &this->a2, nullptr, 1.0, 0.0, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "x", P_FLAG_NONE, &this->x, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "y", P_FLAG_NONE, &this->y, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, .5), &this->param_tree);
    ParamUtils::insert(Param::new_param_bool(
        "thickoutline", P_FLAG_NONE, &this->thickOutline, 1, 0, 0), &this->param_tree);
    ParamUtils::insert(Param::new_param_bool(
        "enabled", P_FLAG_NONE, &this->enabled, 1, 0, 0), &this->param_tree);
    ParamUtils::insert(Param::new_param_int(
        "sides", P_FLAG_NONE, &this->sides, 100, 3, 3), &this->param_tree);
    ParamUtils::insert(Param::new_param_bool(
        "additive", P_FLAG_NONE, &this->additive, 1, 0, 0), &this->param_tree);
    ParamUtils::insert(Param::new_param_bool(
        "textured", P_FLAG_NONE, &this->textured, 1, 0, 0), &this->param_tree);
    ParamUtils::insert(Param::new_param_int(
        "num_inst", P_FLAG_NONE, &this->num_inst, 1000, 1, 1), &this->param_tree);
    ParamUtils::insert(Param::new_param_int(
        "instance", P_FLAG_READONLY, &this->instance, 999, 0, 0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "rad", P_FLAG_NONE, &this->radius, nullptr, MAX_DOUBLE_SIZE, 0, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "ang", P_FLAG_NONE, &this->ang, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "tex_zoom", P_FLAG_NONE, &this->tex_zoom, nullptr, MAX_DOUBLE_SIZE, .00000000001, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "tex_ang", P_FLAG_NONE, &this->tex_ang, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t1", P_FLAG_TVAR, &this->t1, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t2", P_FLAG_TVAR, &this->t2, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t3", P_FLAG_TVAR, &this->t3, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t4", P_FLAG_TVAR, &this->t4, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t5", P_FLAG_TVAR, &this->t5, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t6", P_FLAG_TVAR, &this->t6, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t7", P_FLAG_TVAR, &this->t7, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    ParamUtils::insert(Param::new_param_float(
        "t8", P_FLAG_TVAR, &this->t8, nullptr, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);

    for (unsigned int i = 0; i < NUM_Q_VARIABLES; i++)
    {
        std::ostringstream os;
        os << "q" << i + 1;
        ParamUtils::insert(Param::new_param_float(
            os.str().c_str(), P_FLAG_QVAR, &this->q[i], nullptr,
            MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0), &this->param_tree);
    }

}

/* Frees a custom shape form object */
CustomShape::~CustomShape()
{
    traverseVector<TraverseFunctors::Delete<PerFrameEqn> >(per_frame_eqn_tree);
    traverse<TraverseFunctors::Delete<InitCond> >(init_cond_tree);
    traverse<TraverseFunctors::Delete<Param> >(param_tree);
    traverse<TraverseFunctors::Delete<InitCond> >(per_frame_init_eqn_tree);
    traverse<TraverseFunctors::Delete<Param> >(text_properties_tree);
}

void CustomShape::loadUnspecInitConds()
{
    InitCondUtils::LoadUnspecInitCond fun(this->init_cond_tree, this->per_frame_init_eqn_tree);
    traverse(param_tree, fun);
}

void CustomShape::evalInitConds()
{
    for (auto& pos: per_frame_init_eqn_tree)
    {
        pos.second->evaluate();
    }

    t1Init = t1;
    t2Init = t2;
    t3Init = t3;
    t4Init = t4;
    t5Init = t5;
    t6Init = t6;
    t7Init = t7;
    t8Init = t8;
}

void CustomShape::Draw(RenderContext& context)
{
    for (int i = 0; i < num_inst; i++)
    {
        t1 = t1Init;
        t2 = t2Init;
        t3 = t3Init;
        t4 = t4Init;
        t5 = t5Init;
        t6 = t6Init;
        t7 = t7Init;
        t8 = t8Init;
        instance = i;

        for (auto& pos: per_frame_eqn_tree)
        {
            pos->evaluate();
        }

        Shape::Draw(context);
    }
}
