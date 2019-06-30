!lua

-- TemperatureScaleScript.lua
-- Adjusts the temperature scale value based on the time of day.

local secondsPerDay = 60 * 60 * 24
local percentOfDay = varget("E:LOCAL TIME", "Number") / secondsPerDay

-- Scale the temperature from midnight to noon and then back to midnight.
local minimumScale = 0.75
local maximumScale = 1.25

local temperatureScale = 1.0

if percentOfDay < 0.5 then
    temperatureScale = minimumScale + ((percentOfDay / 0.5) * (maximumScale - minimumScale))
else
    temperatureScale = minimumScale + (((1.0 - percentOfDay) / 0.5) * (maximumScale - minimumScale))
end

varset("T:TemperatureScale", "Number", temperatureScale)
