// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "Render/Render.h"
#include "Components/ComTransform.h"
#include "Components/ComSpline.h"
#include "Game/GameWorld.h"

BE_NAMESPACE_BEGIN

OBJECT_DECLARATION("Spline", ComSpline, Component)
BEGIN_EVENTS(ComSpline)
END_EVENTS
BEGIN_PROPERTIES(ComSpline)
    PROPERTY_BOOL("loop", "Loop", "", false, PropertyInfo::Editor),
    PROPERTY_OBJECT("points", "Points", "points", Guid::zero, ComTransform::metaObject, PropertyInfo::Editor | PropertyInfo::IsArray),
END_PROPERTIES

#ifdef NEW_PROPERTY_SYSTEM
void ComSpline::RegisterProperties() {
    REGISTER_ACCESSOR_PROPERTY("Loop", bool, IsLoop, SetLoop, false, "", PropertyInfo::Editor);
    REGISTER_MIXED_ACCESSOR_PROPERTY("Points", ObjectRefArray, GetPointsRef, SetPointsRef, ObjectRefArray(ComTransform::metaObject, {Guid::zero}), "", PropertyInfo::Editor);
}
#endif

ComSpline::ComSpline() {
    originCurve = nullptr;
    anglesCurve = nullptr;
    curveUpdated = false;

#ifndef NEW_PROPERTY_SYSTEM
    Connect(&Properties::SIG_PropertyChanged, this, (SignalCallback)&ComSpline::PropertyChanged);
#endif
}

ComSpline::~ComSpline() {
    Purge(false);
}

void ComSpline::Purge(bool chainPurge) {
    SAFE_DELETE(originCurve);
    SAFE_DELETE(anglesCurve);

    pointGuids.Clear();

    curveUpdated = false;
}

void ComSpline::Init() {
    Component::Init();

    if (!originCurve) {
        originCurve = new Curve_BSpline<Vec3>();
    }

    if (!anglesCurve) {
        anglesCurve = new Curve_BSpline<Angles>();
    }

#ifndef NEW_PROPERTY_SYSTEM
    loop = props->Get("loop").As<bool>();
#endif

    // Mark as initialized
    SetInitialized(true);
}

void ComSpline::Awake() {
    UpdateCurve();
}

Vec3 ComSpline::GetCurrentOrigin(float time) const {
    if (loop) {
        Wrap(time, 0.0f, 1.0f);
    } else {
        Clamp(time, 0.0f, 1.0f);
    }
    const ComTransform *transform = GetEntity()->GetTransform();
    return transform->GetWorldMatrix() * originCurve->GetCurrentValue(time);
}

Mat3 ComSpline::GetCurrentAxis(float time) const {
    if (loop) {
        Wrap(time, 0.0f, 1.0f);
    } else {
        Clamp(time, 0.0f, 1.0f);
    }
    const ComTransform *transform = GetEntity()->GetTransform();
    return transform->GetWorldMatrix().ToMat3() * anglesCurve->GetCurrentValue(time).ToMat3();
}

ObjectRefArray ComSpline::GetPointsRef() const {
    ObjectRefArray pointsRef(ComTransform::metaObject);
    pointsRef.objectGuids.SetCount(pointGuids.Count());

    for (int i = 0; i < pointGuids.Count(); i++) {
        pointsRef.objectGuids[i] = pointGuids[i];
    }

    return pointsRef;
}

void ComSpline::SetPointsRef(const ObjectRefArray &pointsRef) {
    UpdateCurve();
}

void ComSpline::UpdateCurve() {
    originCurve->Clear();
    anglesCurve->Clear();

    // Disconnect with previously connected points
    for (int pointIndex = 0; pointIndex < pointGuids.Count(); pointIndex++) {
        const Guid pointTransformGuid = pointGuids[pointIndex];
        if (pointTransformGuid.IsZero()) {
            continue;
        }

        ComTransform *pointTransform = (ComTransform *)ComTransform::FindInstance(pointTransformGuid);
        if (!pointTransform) {
            continue;
        }

        pointTransform->Disconnect(&ComTransform::SIG_TransformUpdated, this);
    }

#ifndef NEW_PROPERTY_SYSTEM
    int numPoints = props->NumElements("points");

    pointGuids.SetCount(numPoints);
#endif

    if (pointGuids.Count() > 1) {
        // incremental time just used for key ordering
        float t = loop ? 0 : 100;

        for (int pointIndex = 0; pointIndex < pointGuids.Count(); pointIndex++, t += 100) {
#ifndef NEW_PROPERTY_SYSTEM
            Str propName = va("points[%i]", pointIndex);

            const Guid pointTransformGuid = props->Get(propName).As<Guid>();
            if (pointTransformGuid.IsZero()) {
                continue;
            }

            pointGuids[pointIndex] = pointTransformGuid;
#else
            const Guid pointTransformGuid = pointGuids[pointIndex];
            if (pointTransformGuid.IsZero()) {
                continue;
            }
#endif

            ComTransform *pointTransform = (ComTransform *)ComTransform::FindInstance(pointTransformGuid);
            if (!pointTransform) {
                continue;
            }

            pointTransform->Connect(&ComTransform::SIG_TransformUpdated, this, (SignalCallback)&ComSpline::PointTransformUpdated, SignalObject::Unique);

            const Vec3 origin = pointTransform->GetLocalOrigin();
            originCurve->AddValue(t, origin);

            Angles angles = pointTransform->GetLocalAngles();
            if (pointIndex > 0) {
                Angles prevAngles = anglesCurve->GetValue(pointIndex - 1);
                for (int i = 0; i < 3; i++) {
                    angles[i] = prevAngles[i] + Math::AngleDelta(angles[i], prevAngles[i]);
                }
            }
            anglesCurve->AddValue(t, angles);
        }

        if (originCurve->GetNumValues() > 1) {
            if (!loop) {
                // duplicate a start point
                originCurve->AddValue(0, originCurve->GetValue(0));
                anglesCurve->AddValue(0, anglesCurve->GetValue(0));

                // duplicate a end point
                originCurve->AddValue(t, originCurve->GetValue(originCurve->GetNumValues() - 1));
                anglesCurve->AddValue(t, anglesCurve->GetValue(anglesCurve->GetNumValues() - 1));

                originCurve->SetBoundaryType(Curve_Spline<Vec3>::ClampedBoundary);
                anglesCurve->SetBoundaryType(Curve_Spline<Angles>::ClampedBoundary);
            } else {
                //originCurve->SetCloseTime(100);
                //anglesCurve->SetCloseTime(100);

                originCurve->SetBoundaryType(Curve_Spline<Vec3>::ClosedBoundary);
                anglesCurve->SetBoundaryType(Curve_Spline<Angles>::ClosedBoundary);
            }

            originCurve->SetConstantSpeed(1.0f);
        }

        for (int i = 0; i < originCurve->GetNumValues(); i++) {
            anglesCurve->SetTime(i, originCurve->GetTime(i));
        }
    }

    curveUpdated = true;
}

float ComSpline::Length() {
    if (!curveUpdated) {
        UpdateCurve();
    }

    return originCurve->GetLengthForTime(1.0f);
}

void ComSpline::DrawGizmos(const SceneView::Parms &viewParms, bool selected) {
    if (!curveUpdated) {
        UpdateCurve();
    }

    if (!selected) {
        return;
    }

    if (originCurve->GetNumValues() < 2) {
        return;
    }

    const ComTransform *transform = GetEntity()->GetTransform();

    RenderWorld *renderWorld = GetGameWorld()->GetRenderWorld();

    // Draw originCurve
    float length = originCurve->GetLengthForTime(1.0f);
    int count = Max((int)Math::Ceil(length / MeterToUnit(1.0f)), 1);
    float dt = 1.0f / count;

    float t = 0.0f;
    while (1) {
        if (originCurve->IsDone(t) || t > 1.0f) {
            break;
        }

        Vec3 p0 = transform->GetWorldMatrix() * originCurve->GetCurrentValue(t);
        Vec3 p1 = transform->GetWorldMatrix() * originCurve->GetCurrentValue(t + dt);

        renderWorld->SetDebugColor(Color4::white, Color4::orange);
        renderWorld->DebugLine(p0, p1, 2, true);

        Mat3 axis = transform->GetWorldMatrix().ToMat3() * anglesCurve->GetCurrentValue(t).ToMat3();

        renderWorld->SetDebugColor(Color4::red, Color4::orange);
        renderWorld->DebugLine(p0, p0 + axis[0] * 30, 2, true);

        renderWorld->SetDebugColor(Color4::green, Color4::orange);
        renderWorld->DebugLine(p0, p0 + axis[1] * 30, 2, true);
        
        renderWorld->SetDebugColor(Color4::blue, Color4::orange);
        renderWorld->DebugLine(p0, p0 + axis[2] * 30, 2, true);

        t += dt;
    }

    // Draw transform points
    renderWorld->SetDebugColor(Color4::blue, Color4::cyan);

    for (int pointIndex = 0; pointIndex < props->NumElements("points"); pointIndex++) {
        Str propName = va("points[%i]", pointIndex);

        const Guid pointTransformGuid = props->Get(propName).As<Guid>();
        if (pointTransformGuid.IsZero()) {
            continue;
        }

        ComTransform *pointTransform = (ComTransform *)ComTransform::FindInstance(pointTransformGuid);
        if (!pointTransform) {
            continue;
        }

        Vec3 pos = pointTransform->GetOrigin();
        renderWorld->DebugOBB(OBB(pos, Vec3(4.0f), pointTransform->GetAxis()), 1.0f, false, true);
    }
}

void ComSpline::PropertyChanged(const char *classname, const char *propName) {
    if (!IsInitialized()) {
        return;
    }

    if (!Str::Cmp(propName, "loop")) {
        SetLoop(props->Get(propName).As<bool>());
        return;
    }

    if (!Str::Cmpn(propName, "points", 6)) {
        UpdateCurve();
        return;
    }

    Component::PropertyChanged(classname, propName);
}

bool ComSpline::IsLoop() const {
    return loop;
}

void ComSpline::SetLoop(bool loop) {
    this->loop = loop;

    UpdateCurve();
}

void ComSpline::PointTransformUpdated(const ComTransform *transform) {
    if (!curveUpdated) {
        UpdateCurve();
    }

    for (int pointIndex = 0; pointIndex < props->NumElements("points"); pointIndex++) {
        Str propName = va("points[%i]", pointIndex);

        if (props->Get(propName).As<Guid>() == transform->GetGuid()) {
            const Vec3 origin = transform->GetLocalOrigin();
            originCurve->SetValue(pointIndex, origin);

            Angles angles = transform->GetLocalAngles();
            if (pointIndex > 0) {
                Angles prevAngles = anglesCurve->GetValue(pointIndex - 1);
                for (int i = 0; i < 3; i++) {
                    angles[i] = prevAngles[i] + Math::AngleDelta(angles[i], prevAngles[i]);
                }
            }
            anglesCurve->SetValue(pointIndex, angles);
        }
    }

    UpdateCurve();
}

BE_NAMESPACE_END
