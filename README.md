# 🌌 08 Final Project - Solar System Engine 🌌

> Solar system built with C++ and NardaEngine that brings the cosmos to your screen!

![Image](https://github.com/user-attachments/assets/4f1c4184-5b91-4b27-bc2e-c4f2cfeb0c4a)

### Author - [@Narda05](https://github.com/Narda05)


## ✨ What Makes This Special?

It's a fully interactive solar system that combines realistic physics with beautiful visuals and intuitive controls. 

### 🎯 Key Highlights

**Complete Solar System** - All 9 planets (yes, including Pluto!) with realistic orbital mechanics  
**Moon System** - Earth's moon with proper orbital dynamics  
**Interactive Controls** - Speed up time, focus on planets, toggle orbital paths  
**Beautiful Visuals** - Authentic planetary textures and immersive space background  


## 🎬 See It In Action And Technical Architecture

### ⚡ Rotation Speed Day
![Image](https://github.com/user-attachments/assets/5efff55e-a5c4-4a35-8c0d-3178f20143ed)
*Observe the Rotation Speed Day in the solar system*

### ⚡ Orbit Speed Year
![Image](https://github.com/user-attachments/assets/00d40af5-5879-4f8d-816b-dfdb2a845b3d)
*Observe the orbital motion of the planets around the Sun throughout the year in the solar system.*

#### 🌞 Rotation Speed (Day) Control Orbit Speed (Year)  Control

The rotation speed (day) determines how fast each planet spins on its own axis. You can adjust this globally for all planets using ImGui sliders, or restore the original, realistic speeds.

The orbit speed (year) determines how fast each planet completes its revolution around the sun. You can adjust this globally for all planets using ImGui sliders, or restore the original, realistic speeds.
```cpp

```cpp
/// --- ImGui: Orbit Speed (Year) Control ---

static bool useCustomSpeeds = false;
ImGui::Checkbox("Original velocity", &useCustomSpeeds);

if (useCustomSpeeds)
{
    // Global speed sliders for all planets
    ImGui::SliderFloat("OrbitSpeed (year)", &gOrbitSpeed, 0.001f, 0.2f);
    ImGui::SliderFloat("RotationSpeed (day)", &gRotationSpeed, 0.001f, 0.2f);
    for (Planet& planet : mPlanets)
    {
        planet.mYearSpeed = gOrbitSpeed;   // <-- Orbit speed (year)
        planet.mDaySpeed = gRotationSpeed; // <-- Rotation speed (day)
    }
}
else
{
    // Restore original speeds for each planet
    for (int i = 0; i < mPlanets.size(); ++i)
    {
        mPlanets[i].mYearSpeed = gOriginalYearSpeeds[i];
        mPlanets[i].mDaySpeed = gOriginalDaySpeeds[i];
    }
}
```
Planet update logic
```cpp
// --- Planet Orbit Update ---

void Planet::Update(float deltaTime)
{
    mCurrentYearRotation += mYearSpeed * deltaTime; // <-- Orbit speed (year)
    mCurrentDayRotation += mDaySpeed * deltaTime;   // <-- Rotation speed (day)

    // Keep angles within 0-2π
    if (mCurrentYearRotation > NardaEngine::Math::TwoPi)
        mCurrentYearRotation -= NardaEngine::Math::TwoPi;
    if (mCurrentDayRotation > NardaEngine::Math::TwoPi)
        mCurrentDayRotation -= NardaEngine::Math::TwoPi;

    // ... update transformation matrices, etc.
}
```

### 🌍  Earth-Moon System Earth and Moon Orbiting Earth
![Image](https://github.com/user-attachments/assets/9b5b50bb-b406-4782-912b-6bc89e0dd6bb)
*Observe how the Moon and the Earth rotate around each other and how each one spins on its own axis*

The moon’s position is calculated relative to the Earth, not the Sun. This ensures the moon orbits the Earth as both move around the Sun.

```cpp
// Get Earth's world matrix and position
const auto& earthMat = mPlanets[2].GetRenderData().matWorld;
NardaEngine::Math::Vector3 earthPos = { earthMat._41, earthMat._42, earthMat._43 };

// Calculate moon's position relative to Earth
float moonAngle = mPlanets[3].mCurrentYearRotation;
float moonDist = mPlanets[3].mOrbitDistance;
NardaEngine::Math::Vector3 moonOffset = {
    cosf(moonAngle) * moonDist,
    0.0f,
    sinf(moonAngle) * moonDist
};

// Build moon's world matrix: first rotate (self-rotation), then translate to orbit position
Math::Matrix4 moonRot = Math::Matrix4::RotationY(mPlanets[3].mCurrentDayRotation);
Math::Matrix4 moonTrans = Math::Matrix4::Translation(earthPos + moonOffset);

NardaEngine::Graphics::SimpleTextureEffect::RenderData moonData = mPlanets[3].GetRenderData();
moonData.matWorld = moonRot * moonTrans;

// Render the moon with the updated transform
mSimpleTextureEffect.Render(moonData);
```

### 🔄 Orbital Path Visualization  Rotation Speed (Day) Control Orbit Speed (Year)  Control
![Image](https://github.com/user-attachments/assets/b8934f6d-442f-48e0-b48b-973fe705188b)
*This control toggles the visibility of the planetary orbits. Useful for observing both the free motion of the planets and their defined orbital paths.*

The "Show Orbits" checkbox in ImGui allows users to toggle the visibility of the orbit rings for all planets. When enabled, the code draws a circular ring for each planet’s orbit (except the moon) using SimpleDraw::AddGroundCircle.


```cpp
// --- ImGui: Show Orbits Toggle ---

ImGui::Checkbox("Show Orbits", &gShowOrbits);
```
Rendering code
```cpp
/ --- Drawing Orbit Rings ---

if (gShowOrbits)
{
    for (size_t i = 0; i < mPlanets.size(); ++i)
    {
        if (i != 3) // Skip the moon
            SimpleDraw::AddGroundCircle(100, mPlanets[i].mOrbitDistance, Colors::Gray, Math::Vector3::Zero);
    }
}
```

### 🎛️ ImGui controls and ImGui Control Panel Key Features
![Image](https://github.com/user-attachments/assets/70ae53cf-0f89-4a30-821a-9d67eb495343)
*Clean and intuitive control interface*

What this code does lets the user toggle orbit rings, user switch between original and custom speeds for all planets, provides sliders to adjust orbit (year) and rotation (day) speeds in real time and lets the user select any planet (or the sun/moon) to focus the camera and render target.

```cpp
// --- ImGui Control Panel: Key Features ---

ImGui::Begin("Solar System Controls");

// Toggle orbit rings visualization
ImGui::Checkbox("Show Orbits", &gShowOrbits);

// Option to use custom or original planet speeds
static bool useCustomSpeeds = false;
ImGui::Checkbox("Original velocity", &useCustomSpeeds);

if (useCustomSpeeds)
{
    // Global speed sliders for all planets
    ImGui::SliderFloat("OrbitSpeed (year)", &gOrbitSpeed, 0.001f, 0.2f);
    ImGui::SliderFloat("RotationSpeed (day)", &gRotationSpeed, 0.001f, 0.2f);
    for (Planet& planet : mPlanets)
    {
        planet.mYearSpeed = gOrbitSpeed;
        planet.mDaySpeed = gRotationSpeed;
    }
}
else
{
    // Restore original speeds for each planet
    for (int i = 0; i < mPlanets.size(); ++i)
    {
        mPlanets[i].mYearSpeed = gOriginalYearSpeeds[i];
        mPlanets[i].mDaySpeed = gOriginalDaySpeeds[i];
    }
}

// --- Select planet to focus ---
ImGui::Combo("Focus Planet", &gSelectedPlanet, gPlanetNames, IM_ARRAYSIZE(gPlanetNames));

// ... (camera focus logic and render target code follows)

ImGui::End();
```

The ImGui panel provides:
-  **Orbit Toggle** - Show/hide planetary paths
-  **Speed Controls** - Adjust orbital and rotational speeds
-  **Planet Focus** - Select any planet for close-up view
-  **Real-time Adjustments** - All changes apply instantly



## 🏗️ Technical Architecture





### 🌍 Planetary Data

| Planet | Radius | Orbit Distance | Day Speed | Year Speed |
|--------|--------|----------------|-----------|------------|
| Mercury | 0.25 | 2.0 | 0.1 | 0.08 |
| Venus | 0.35 | 3.0 | 0.03 | 0.06 |
| Earth | 0.40 | 4.0 | 0.1 | 0.04 |
| Moon | 0.12 | 0.7* | 0.2 | 0.2 |
| Mars | 0.30 | 5.5 | 0.08 | 0.03 |
| Jupiter | 0.80 | 8.0 | 0.2 | 0.015 |
| Saturn | 0.70 | 9.0 | 0.15 | 0.010 |
| Uranus | 0.50 | 10.0 | 0.12 | 0.007 |
| Neptune | 0.50 | 11.0 | 0.10 | 0.005 |
| Pluto | 0.15 | 12.0 | 0.05 | 0.003 |

*Moon orbit distance is relative to Earth



