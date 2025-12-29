local speed = 8

function Start()
  print("howdee")
end

--- @param _ number
function Update(_)
  print("i am going" .. speed .. "units per second")
end

return {
  speed
}
