subdir('driver')
subdir('general_purpose')
subdir('measurement')

model_source = [driver_source, general_purpose_source, measurement_source, 'event_bus.cpp']

model_lib = static_library('model',
                           model_source,
                           include_directories : inc)
