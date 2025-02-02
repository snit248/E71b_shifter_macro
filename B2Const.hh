#ifndef ___CONSTH
#define ___CONSTH 1

#include <G4SystemOfUnits.hh>
#include <cstddef>

static const int B2_NON_INITIALIZED_VALUE = -2;

/////////////////////////////////////////////////////////
//                        NOTE                         //
/////////////////////////////////////////////////////////

// The following naming conventions are applied:

// Proton module constants always begin with PM
// WAGASCI constants always begin with WGS
// WallMRD constants always begin with WM
// BabyMIND constants always begin with BM

// Constants ending with _WIDTH refer to the X direction (exceptions are alwyas notified)
// Constants ending with _HEIGHT refer to the Y direction (exceptions are alwyas notified)
// Constants ending with _DEPTH refer to the Z direction (exceptions are alwyas notified)
// Constants ending with _LENGTH, _THICK, _DISTANCE, _GAP, _LARGE do not refer to any particular direction
// Constants containing NUM are of integer type (number of something)
// Constants containing START refer to the starting position of something
// Constants containing END refer to the starting position of something

// max number of views
static const std::size_t NUM_VIEWS = 2;
// max number of planes
static const std::size_t MAX_NUM_PLANES = 80;
// max number of channels in a plane
static const std::size_t MAX_NUM_CHANNELS = 100;
// max number of emulsion gel layers in one film
static const std::size_t DUMAX = 2;
// max number of ECC
static const std::size_t ECCMAX = 9;
// max number of emulsion films in one ECC
static const std::size_t GELMAX = 133;

/////////////////// BEAM ////////////////////

// number if bunches in a spill
static const std::size_t NUM_BUNCHES = 8;
// nominal bunch to bunch period in ns
const double NOMINAL_BUNCH_PERIOD_NS = 581 * ns;
// nominal bunch width in ns
const double NOMINAL_BUNCH_WIDTH_NS = 60 * ns;
// default branch of MC simulated hits
const int DEFAULT_MC_BUNCH_ID = 0;
// default time window for simulating dark noise hits in a spill
const double DEFAULT_DARK_NOISE_TIME_WINDOW = 10 * us;
// max time window for simulating dark noise hits in a spill
const double MAX_DARK_NOISE_TIME_WINDOW = 1 * s;

/////////////////// B2 hall ////////////////////

// maximum width (X direction) of the simulated experimental hall
const double B2_WORLD_WIDTH = 50 * m; // m
// maximum height (Y direction) of the simulated experimental hall
const double B2_WORLD_HEIGHT = 50 * m; // m
// maximum depth (Z direction) of the simulated experimental hall
const double B2_WORLD_DEPTH = 50 * m; // m
// thickness of the B2 hall wall
// const double HALL_RADIUS_THICK = 8. * m; // m
const double HALL_RADIUS_THICK = 12. * m; // m
// minimum radius of the B2 hall
const double HALL_RADIUS_MIN = 8.95 * m; // m
// maximum radius of the B2 hall
const double HALL_RADIUS_MAX = HALL_RADIUS_MIN + HALL_RADIUS_THICK;
// Width of the pillar
const double PILLAR_WIDTH = 1.0 * m;
// Height of the pillar
const double PILLAR_HEIGHT = 3.0 * m;
// Depth of the pillar
const double PILLAR_DEPTH = 4.0 * m;
// Depth of wall for MC factor calculation
const double WALL_DEPTH = HALL_RADIUS_THICK;

/////////////////// Common to all detectors ////////////////////

// Transition speed in generic fiber = 280 [mm/s]
const double TRANS_SPEED_IN_FIBER = 280 * (mm / ns);
// Transition speed in WallMRD fiber 52.6 [mm/s]
//const double TRANS_SPEED_IN_FIBER_WMRD = 52.6 * (mm / ns);
const double TRANS_SPEED_IN_FIBER_WMRD = 157.8 * (mm / ns);

/////////////////// Proton Module ////////////////////

// The origin (relative to the Proton Module) is placed in the center of the detector.

// Number of front VETO planes (non double counting top view and side view planes)
const int PM_NUM_FRONT_VETO_PLANES = 1;
// Number of non front VETO planes (up/down/left/right)
const int PM_NUM_NON_FRONT_VETO_PLANES = 4;
// Number of tracking planes in the inner detector (excluding VETO planes)
// (non double counting top view and side view planes)
const int PM_NUM_TRACKING_PLANES = 17;
// Total number of planes (non double counting top view and side view planes)
const int PM_NUM_PLANES = PM_NUM_FRONT_VETO_PLANES + PM_NUM_NON_FRONT_VETO_PLANES + PM_NUM_TRACKING_PLANES;
// Number of channels per tracking non veto plane
const int PM_NUM_CHANNELS_ONE_TRACK_PLANE = 32;
// Number of channels of the front VETO planes
const int PM_NUM_CHANNELS_FRONT_VETO = 24;
// Number of channels for other VETO planes
const int PM_NUM_CHANNELS_OTHER_VETO = 17;

// Proton module box width
const double PM_BOX_WIDTH = 141.8 * cm;
// Proton module box height
const double PM_BOX_HEIGHT = 141.8 * cm;
// Proton module box depth
const double PM_BOX_DEPTH = 91.9 * cm;

// Proton module cycle duration
const double PM_CYCLE_DURATION = NOMINAL_BUNCH_PERIOD_NS;
// Proton module cycle offset
const int PM_CYCLE_OFFSET = 23;

// Proton module scintillator bars

// Length of Proton Module long VETO scintillator bars
const double PM_LONG_VETO_SCINTI_LENGTH = 125 * cm;
// Length of Proton Module short VETO scintillator bars
const double PM_SHORT_VETO_SCINTI_LENGTH = 120.3 * cm;
// Length of Proton Module scintillator bars
const double PM_SCINTI_BAR_LENGTH = 120.3 * cm;
// Depth of the INGRID type scintillator bars
const double PM_INGRID_SCINTI_THICK = 1.0 * cm;
// Depth of the SciBar type scintillator bars
const double PM_SCIBAR_SCINTI_THICK = 1.3 * cm;
// Largeness of SciBar-type scintillators
const double PM_SCIBAR_SCINTI_LARGE = 2.5 * cm;
// Largeness of INGRID-type scintillators
const double PM_INGRID_SCINTI_LARGE = 5.0 * cm;

// Proton module active area

// Height of the PM active area
const double PM_TOTAL_ACTIVE_AREA_HEIGHT = PM_SCINTI_BAR_LENGTH;
// Width of the PM active area
const double PM_TOTAL_ACTIVE_AREA_WIDTH = PM_SCINTI_BAR_LENGTH;
// Height of the PM inner region area (where there are only INGRID scintillators)
const double PM_SCIBAR_ACTIVE_AREA_HEIGHT = 8. * PM_INGRID_SCINTI_LARGE;
// Width of the PM inner region area (where there are only INGRID scintillators)
const double PM_SCIBAR_ACTIVE_AREA_WIDTH = 8. * PM_INGRID_SCINTI_LARGE;
// Total thickness (in the Z direction) of the INGRID/SciBar scintillators
// mixed region not counting the first two VETO planes and air gaps
const double PM_MIXED_REGION_THICKNESS =
  PM_NUM_TRACKING_PLANES * (PM_SCIBAR_SCINTI_THICK + PM_INGRID_SCINTI_THICK);
// Total thickness (in the Z direction) of the SciBar-type scintillators
// only region not counting the first two VETO planes and air gaps
const double PM_SCIBAR_REGION_THICKNESS =
  PM_NUM_TRACKING_PLANES * (2. * PM_SCIBAR_SCINTI_THICK);
// Total thickness (in the Z direction) of the INGRID-type scintillators only
// region not counting the first two VETO planes and air gaps
const double PM_INGRID_REGION_THICKNESS =
  PM_NUM_TRACKING_PLANES * (2. * PM_INGRID_SCINTI_THICK);
// Total thickness (in the Z direction) of the front VETO planes
const double PM_VETO_REGION_THICKNESS = 2. * PM_INGRID_SCINTI_THICK;

// Proton module distances

// Start of the first plane scintillator (Z coordinate)
const double PM_FRONT_VETO_START_Z = -0.5 * PM_BOX_DEPTH + 5. * cm;
// Start of the first (non front) VETO scintillator (Z coordinate)
const double PM_NON_FRONT_VETO_START_Z = -0.5 * PM_BOX_DEPTH + 3.45 * cm;
// Start of the front veto and tracking plane XY
// 12mai * INGRID scintillator = 8mai * SciBar + 8mai * INGRID scintillator
const double PM_SCINTI_START_XY = 12. * PM_INGRID_SCINTI_LARGE;
// distance between the upstream surfaces of two planes (all but the 0Y and 1X planes)
const double PM_NEXT_PLANE_DISTANCE = 2.3 * cm;
// distance between the upstream surfaces of 0Y and 1X planes
const double PM_0Y_1X_PLANE_DISTANCE = 2.7 * cm;
// Y position of the upper VETO in PM box
const double PM_VETO_UP_Y = 0.5 * PM_BOX_HEIGHT - 41.5 * mm
  - 0.5 * PM_INGRID_SCINTI_THICK;
// X position of the bottom VETO in PM box
const double PM_VETO_BOTTOM_X = -0.5 * PM_BOX_WIDTH + 97.5 * mm
  + 0.5 * PM_SHORT_VETO_SCINTI_LENGTH;
// Y position of the bottom VETO in PM box
const double PM_VETO_BOTTOM_Y = -0.5 * PM_BOX_HEIGHT + 33.5 * mm
  + 0.5 * PM_INGRID_SCINTI_THICK;
// X position of the left VETO in PM box
// left and right are determined from the upstream view
const double PM_VETO_LEFT_X = 0.5 * PM_BOX_WIDTH - 41.5 * mm;
// X position of the right VETO in PM box
const double PM_VETO_RIGHT_X = -0.5 * PM_BOX_WIDTH + 42.5 * mm;

// Total depth of the frame (VETO planes + tracking planes)
const double PM_TOTAL_FRAME_DEPTH =
  PM_NEXT_PLANE_DISTANCE // First VETO plane (plane 0X)
  + PM_0Y_1X_PLANE_DISTANCE // Second VETO plane (plane 0Y)
  + PM_NUM_TRACKING_PLANES * PM_NEXT_PLANE_DISTANCE * 2 // tracking planes
  + 0.5 * PM_INGRID_SCINTI_THICK
  + 0.5 * PM_SCIBAR_SCINTI_THICK; // last tracking plane (plane 17Y) depth

/////////////////// WAGASCI /////////////////////

// Timing
// The WAGASCI cycle corresponds to the WAGASCI BCID, the offset
// between the beam trigger and the first bunch is 23 cycles
const int WGS_CYCLE_OFFSET = 23;
// Duration of acquisition time window of an event for simulated dark-noise
const double WGS_ACQUISITION_WINDOW_DURATION = 250 * millisecond;
// Duration of a BCID count
const double WGS_BCID_DURATION = NOMINAL_BUNCH_PERIOD_NS;

// Cross-talk
// Cross-talk rate of WAGASCI upstream
// It is tuned so that the we can reproduce the plot taken from Taichiro Koga PhD thesis (page 54)
const double WGS_UP_CROSS_TALK_RATE = 1.33;
// Cross-talk rate of WAGASCI downstream
// // It is tuned so that the we can reproduce the plot taken from Chikuma Naruhiro PhD thesis (page 125)
const double WGS_DOWN_CROSS_TALK_RATE = 1.25;

// Number of slots for plane scintillators
const int WGS_NUM_SLOTS_PLANE = 40;
// Number of slots for grid left scintillators
const int WGS_NUM_SLOTS_GRID_LEFT = 20;
// Number of slots for grid right scintillators
const int WGS_NUM_SLOTS_GRID_RIGHT = 20;
// Number of slots for grid scintillators
const int WGS_NUM_SLOTS_GRID = WGS_NUM_SLOTS_GRID_LEFT + WGS_NUM_SLOTS_GRID_RIGHT;
// Total number of slots
const int WGS_NUM_SLOTS = WGS_NUM_SLOTS_PLANE + WGS_NUM_SLOTS_GRID;
// Number of planes
const int WGS_NUM_PLANES = 8;
// Max grid number
const int WGS_MAX_GRID_NUMBER = 2;

// WAGASCI box width
const double WGS_BOX_WIDTH = 127.6 * cm;
// WAGASCI box height
const double WGS_BOX_HEIGHT = 120.4 * cm;
// WAGASCI box depth
const double WGS_BOX_DEPTH = 51 * cm;

// Steel box width
const double WGS_STEEL_BOX_WIDTH = WGS_BOX_WIDTH;
// Steel box height
const double WGS_STEEL_BOX_HEIGHT = WGS_BOX_HEIGHT;
// Steel box depth
const double WGS_STEEL_BOX_DEPTH = WGS_BOX_DEPTH;

// Water box width
const double WGS_WATER_BOX_WIDTH = 125.2 * cm;
// Water box height
const double WGS_WATER_BOX_HEIGHT = 117.2 * cm;
// Water box depth
const double WGS_WATER_BOX_DEPTH = 48.2 * cm;

// upstream water surface Z coordinate : half of container length
const double WGS_FRAME_START_Z = - 0.5 * WGS_WATER_BOX_DEPTH;
// distance between water target and scintillator
const double WGS_FIRST_DISTANCE_Z = 6.5 * mm;
// distance between water target and scintillator
const double WGS_OFFSET_Z = WGS_FRAME_START_Z + WGS_FIRST_DISTANCE_Z;
// offset of the plane scintillator start from the WAGASCI sub-module center
const double WGS_OFFSET_XY_PLANE = -50. * cm;
// X offset of the scintillator sub modules to the WAGASCI water box
const double WGS_SCI_OFFSET_TO_WATER_X = -1.1 * cm;
// Y offset of the scintillator sub modules to the WAGASCI water box
const double WGS_SCI_OFFSET_TO_WATER_Y = 4.1 * cm;

// Length of the WAGASCI scintillator bars
const double WGS_SCINTI_BAR_LENGTH = 102 * cm;
// thickness of scintillator
const double WGS_SCINTI_THICK = 0.3 * cm;
// width of scintillator
const double WGS_SCINTI_LARGE = 2.5 * cm;
// gap between two consecutive grid scintillators on the same plane
const double WGS_GRID_SPACING = 5.0 * cm;
// thickness of the cut into a grid scintillator
const double WGS_CUT_THICKNESS = 3.23 * mm;
// depth of the cut into a grid scintillator
const double WGS_CUT_DEPTH = 13. * mm;
// offset of the first cut of a grid scintillator from the center
const double WGS_OFFSET_XY_GRID = -0.5 * WGS_SCINTI_BAR_LENGTH + 3.5 * cm;
// Fiber length between scinti and bundle for WAGASCI (cm)
const double WGS_FIBER_BUNDLE_LENGTH = 20 * centimeter;

//////////////////// WallMRD /////////////////////

// Number of scintillator planes
const int WM_NUM_SCINTI_PLANES = 10;
// Number of iron planes in the inner frame
const int WM_NUM_INNER_IRON_PLANES = 9;
// Number of iron planes making the frame walls
const int WM_NUM_OUTER_IRON_PLANES = 2;
// Number of iron planes making the side walls
const int WM_NUM_SIDE_IRON_PLANES = 2;
// Total number of iron planes
const int WM_NUM_IRON_PLANES = WM_NUM_INNER_IRON_PLANES + WM_NUM_OUTER_IRON_PLANES
  + WM_NUM_SIDE_IRON_PLANES;
// number of scintillator bars in a plane
const int WM_NUM_SCINTI_IN_ONE_PLANE = 8;

// WallMRD box width
const double WM_BOX_WIDTH = 46. * cm;
// WallMRD box height
const double WM_BOX_HEIGHT = 205. * cm;
// WallMRD box depth
const double WM_BOX_DEPTH = 163. * cm;

// Thickness of the inner iron plates
const double WM_INNER_IRON_PLATE_THICK = 3. * cm;
// Height of the inner iron plates
const double WM_INNER_IRON_PLATE_HEIGHT = 180. * cm;
// Largeness of the inner iron plates
const double WM_INNER_IRON_PLATE_LARGE = 161. * cm;
// Thickness of the front and rear iron plates
const double WM_FRONT_IRON_PLATE_THICK = 3. * cm;
// Height of the front and rear iron plates
const double WM_FRONT_IRON_PLATE_HEIGHT = 205. * cm;
// Largeness of the front and rear iron plates
const double WM_FRONT_IRON_PLATE_LARGE = 161. * cm;
// Thickness of the side iron plates
const double WM_SIDE_IRON_PLATE_THICK = 1. * cm;
// Height of the side iron plates
const double WM_SIDE_IRON_PLATE_HEIGHT = 190. * cm;
// Largeness of the side iron plates
const double WM_SIDE_IRON_PLATE_LARGE = 46. * cm;

// Largeness of WallMRD scintillator
const double WM_SCINTI_LARGE = 20.0 * cm;
// Length of WallMRD scintillator
const double WM_SCINTI_LENGTH = 180 * cm;
// Thickness of WallMRD scintillator
const double WM_SCINTI_THICK = 0.7 * cm;
// Gap between two iron planes. Notice that the gap of 1.3 cm is wider
// than the scintillator width of 0.7 cm
const double WM_IRON_PLANES_GAP = 1.3 * cm;
// total nominal width (in the x direction) of the WallMRD frame
// 11 planes of iron plates with 3cm width plus 10 planes of scintillator with a gap of 1.3 cm
const double WM_INNER_FRAME_WIDTH = WM_NUM_IRON_PLANES * WM_INNER_IRON_PLATE_THICK
  + WM_NUM_SCINTI_PLANES * WM_IRON_PLANES_GAP;
// height (in the y direction) of the WallMRD inner frame where the scintillators are
const double WM_INNER_FRAME_HEIGHT = WM_SCINTI_LENGTH;
// depth (in the z direction) of the WallMRD inner frame where the scintillators are
const double WM_INNER_FRAME_DEPTH = WM_NUM_SCINTI_IN_ONE_PLANE * WM_SCINTI_LARGE;
// Z offset of the WallMRD inner frame where scintillators are
const double WM_INNER_FRAME_START_Z = - 0.5 * WM_INNER_FRAME_DEPTH;
// Y offset of inner iron and scintillator in the module box
const double WM_INNER_OFFSET_Y = -2.5 * cm;
// Y offset of side iron in the module box
const double WM_SIDE_IRON_OFFSET_Y = -7.5 * cm;
// Z offset of side iron in the module box
const double WM_SIDE_IRON_OFFSET_Z = 0.5 * WM_BOX_DEPTH - 0.5 * WM_SIDE_IRON_PLATE_THICK;
// gap between WallMRD scintillator planes
const double WM_PLANES_GAP = WM_IRON_PLANES_GAP + WM_INNER_IRON_PLATE_THICK;
// start iron plane X
const double WM_START_IRON_X = -0.5 * WM_BOX_WIDTH + 0.5 * WM_FRONT_IRON_PLATE_THICK;
// start scintillator Z
const double WM_START_SCINTI_Z = 80. * cm;
// WallMRD start offset X
const double WM_START_OFFSET_X = 6 * mm;
// global offset X (position of the first plane scintillator in the module box)
const double WM_GLOBAL_OFFSET_X = -0.5 * WM_BOX_WIDTH + WM_FRONT_IRON_PLATE_THICK
  + 0.5 * WM_IRON_PLANES_GAP;
// global offset Z (position of the first plane scintillator in the module box)
const double WM_GLOBAL_OFFSET_Z = -0.5 * WM_BOX_DEPTH + WM_SIDE_IRON_PLATE_THICK + 0.5 * WM_SCINTI_LARGE;
// fiber length in each section
const double WM_FIBER_LENGTH_IN_SECTION = 36. * cm;

//////////////////// BabyMIND /////////////////////

// WallMRD box width
const double BM_BOX_WIDTH = 400 * cm;
// WallMRD box height
const double BM_BOX_HEIGHT = 400 * cm;
// WallMRD box depth
const double BM_BOX_DEPTH = 432 * cm;
// number of iron planes
const static std::size_t BM_NUM_IRON_PLANES = 33;
// Z coordinate of all the iron planes central point
// offset on the Z coordinate on the output of the B2Dimension::GetPosBm method
const double BABYMIND_OFFSET_Z = -2.0 * m;
const double BM_IRON_MODULE_Z[BM_NUM_IRON_PLANES] = {70 * mm + BABYMIND_OFFSET_Z,
                                                     142 * mm + BABYMIND_OFFSET_Z,
                                                     210 * mm + BABYMIND_OFFSET_Z,
                                                     497 * mm + BABYMIND_OFFSET_Z,
                                                     607 * mm + BABYMIND_OFFSET_Z,
                                                     717 * mm + BABYMIND_OFFSET_Z,
                                                     959 * mm + BABYMIND_OFFSET_Z,
                                                     1029 * mm + BABYMIND_OFFSET_Z,
                                                     1137 * mm + BABYMIND_OFFSET_Z,
                                                     1210 * mm + BABYMIND_OFFSET_Z,
                                                     1452 * mm + BABYMIND_OFFSET_Z,
                                                     1519 * mm + BABYMIND_OFFSET_Z,
                                                     1631 * mm + BABYMIND_OFFSET_Z,
                                                     1700 * mm + BABYMIND_OFFSET_Z,
                                                     1938 * mm + BABYMIND_OFFSET_Z,
                                                     2008 * mm + BABYMIND_OFFSET_Z,
                                                     2078 * mm + BABYMIND_OFFSET_Z,
                                                     2148 * mm + BABYMIND_OFFSET_Z,
                                                     2437 * mm + BABYMIND_OFFSET_Z,
                                                     2506 * mm + BABYMIND_OFFSET_Z,
                                                     2578 * mm + BABYMIND_OFFSET_Z,
                                                     2865 * mm + BABYMIND_OFFSET_Z,
                                                     2937 * mm + BABYMIND_OFFSET_Z,
                                                     3004 * mm + BABYMIND_OFFSET_Z,
                                                     3074 * mm + BABYMIND_OFFSET_Z,
                                                     3313 * mm + BABYMIND_OFFSET_Z,
                                                     3383 * mm + BABYMIND_OFFSET_Z,
                                                     3454 * mm + BABYMIND_OFFSET_Z,
                                                     3521 * mm + BABYMIND_OFFSET_Z,
                                                     3760 * mm + BABYMIND_OFFSET_Z,
                                                     3833 * mm + BABYMIND_OFFSET_Z,
                                                     3902 * mm + BABYMIND_OFFSET_Z,
                                                     3971 * mm + BABYMIND_OFFSET_Z};
// first iron layer where the neutrino interaction may occurs (counting from 0)
const int BM_FIRST_INTERACTION_LAYER = 0;
// last iron layer where the neutrino interaction may occurs
const int BM_LAST_INTERACTION_LAYER = 32;
// Number of tracking planes
const int BM_NUM_TRACKING_PLANES = 18;
// Number of horizontal scintillators in a plane
const int BM_NUM_HORIZ_SCINTI_ONE_PLANE = 95;
// Number of vertical scintillators in a plane
const int BM_NUM_VERTICAL_SCINTI_ONE_PLANE = 16;
// width (x direction) of the BabyMIND iron plate
const double BM_IRON_PLATE_WIDTH = 350. * cm;
// height (y direction) of the BabyMIND iron plate
const double BM_IRON_PLATE_HEIGHT = 200. * cm;
// depth (z direction) of the BabyMIND iron plate
const double BM_IRON_PLATE_DEPTH = 3.0 * cm;
// Total depth of BabyMIND in the Z direction // TODO to check
const double BM_FRAME_DEPTH = BM_IRON_MODULE_Z[BM_NUM_IRON_PLANES - 1] + 0.5 * BM_IRON_PLATE_DEPTH;
// Start of the BabyMIND upstream surface with respect to relative coordinate system
const double BM_FRAME_START_Z = - 0.5 * BM_FRAME_DEPTH;
// Length of horizontal scintillators
const double BM_HORIZONTAL_SCINTI_LENGTH = 288. * cm;
// Thickness of horizontal scintillators
const double BM_HORIZONTAL_SCINTI_THICK = 0.75 * cm;
// Largeness of horizontal scintillators
const double BM_HORIZONTAL_SCINTI_LARGE = 3.13 * cm;
// Length of vertical scintillators
const double BM_VERTICAL_SCINTI_LENGTH = 195. * cm;
// Thickness of vertical scintillators
const double BM_VERTICAL_SCINTI_THICK = 0.75 * cm;
// Largeness of vertical scintillators
const double BM_VERTICAL_SCINTI_LARGE = 21.06 * cm;

///////////////////// YASU tracker //////////////////////

// Number of planes
const int YASU_NUM_PLANES = 2;
// Number of channels in a plane
const int YASU_NUM_SLOTS_ONE_PLANE = 14;
// Position of the first YASU plane
const double YASU_POS_Z0 = 10 * cm;
// Position of the second YASU plane
const double YASU_POS_Z1 = 17 * cm;

//////////////////// NINJA /////////////////////

// Number of NINJA traker plane
const int NINJA_TRACKER_NUM_PLANES = 4;
// Number of NINJA tracker channels in a plane
const int NINJA_TRACKER_NUM_CHANNELS_ONE_PLANE = 31;
// Number of NINJA ECC water layers
const int NINJA_ECC_WATER_LAYERS = 58;
// Number of NINJA ECC iron layers
const int NINJA_ECC_IRON_LAYERS = 70;
// NINJA rack width
const double NINJA_RACK_WIDTH = 140. * cm;
// NINJA rack height
const double NINJA_RACK_HEIGHT = 230. * cm;
// NINJA rack depth
const double NINJA_RACK_DEPTH = 40. * cm;
// NINJA rack pillar width
const double NINJA_RACK_PILLAR_WIDTH = 4.5 * cm;
// NINJA rack pillar height
const double NINJA_RACK_PILLAR_HEIGHT = NINJA_RACK_HEIGHT;
// NINJA rack pillar depth
const double NINJA_RACK_PILLAR_DEPTH = 5.5 * cm;
// NINJA rack plate width
const double NINJA_RACK_PLATE_WIDTH = NINJA_RACK_WIDTH - 2 * NINJA_RACK_PILLAR_WIDTH;
// NINJA rack plate height
const double NINJA_RACK_PLATE_HEIGHT = 4. * cm;
// NINJA rack plate depth
const double NINJA_RACK_PLATE_DEPTH = NINJA_RACK_DEPTH - 2 * NINJA_RACK_PILLAR_DEPTH;
// NINJA tracker width/height
const double NINJA_TRACKER_XY = 130. * cm;
// NINJA tracker depth
const double NINJA_TRACKER_DEPTH = 4. * cm;
// NINJA tracker frame thickness
const double NINJA_TRACKER_FRAME_THICK = 4. * cm;
// NINJA tracker offset (rack outer edge - tracker inner edge)
const double NINJA_TRACKER_OFFSET = 1.8 * cm;
// NINJA box width
const double NINJA_BOX_WIDTH = NINJA_RACK_WIDTH;
// NINJA box height
const double NINJA_BOX_HEIGHT = NINJA_RACK_HEIGHT;
// NINJA box depth
const double NINJA_BOX_DEPTH = NINJA_RACK_DEPTH + NINJA_TRACKER_DEPTH - NINJA_TRACKER_OFFSET;
// PVC thin pillar width
const double NINJA_PVC_THIN_PILLAR_WIDTH = 1.2 * cm;
// PVC thick pillar width
const double NINJA_PVC_THICK_PILLAR_WIDTH = 7.5 * cm;
// PVC pillar height
const double NINJA_PVC_PILLAR_HEIGHT = NINJA_TRACKER_XY - 2 * NINJA_TRACKER_FRAME_THICK;
// PVC pillar depth
const double NINJA_PVC_PILLAR_DEPTH = 1. * cm;
// PVC plate width
const double NINJA_PVC_PLATE_WIDTH = 104.6 * cm;
// PVC large plate height
const double NINJA_PVC_LARGE_PLATE_HEIGHT = 12.6 * cm;
// PVC small plate height
const double NINJA_PVC_SMALL_PLATE_HEIGHT = 10. * cm;
// PVC plate depth
const double NINJA_PVC_PLATE_DEPTH = 3. * mm;
// PVC plate gap
const double NINJA_PVC_PLATE_GAP = 36. * cm;
// From PVC thick pillar to frame inner edge
const double NINJA_PVC_REMAIN_X = NINJA_PVC_PILLAR_HEIGHT - NINJA_PVC_THICK_PILLAR_WIDTH
  - NINJA_PVC_PLATE_WIDTH - NINJA_PVC_THIN_PILLAR_WIDTH;
// From PVC small plate edge to frame inner edge
const double NINJA_PVC_REMAIN_Y = NINJA_PVC_PILLAR_HEIGHT - NINJA_PVC_LARGE_PLATE_HEIGHT
  - 2 * NINJA_PVC_SMALL_PLATE_HEIGHT - 2 * NINJA_PVC_PLATE_GAP;
// NINJA horizontal scintillator length
const double NINJA_HORIZ_SCI_LENGTH = 966. * mm;
// NINJA vertical scintillator length
const double NINJA_VERTI_SCI_LENGTH = 1012. * mm;
// NINJA scintillator width
const double NINJA_SCI_WIDTH = 2.4 * cm;
// NINJA scintillator gap
const double NINJA_SCI_GAP = 2.5 * cm;
// NINJA scintillator thickness
const double NINJA_SCI_THICK = 3. * mm;
// NINJA vertical Y / horizontal X offset in the tracker module box
const double NINJA_SCI_OFFSET_X = 0.5 * NINJA_PVC_PILLAR_HEIGHT - NINJA_PVC_REMAIN_Y;
// NINJA vertical X / horizontal Y offset in the tracker module box
const double NINJA_SCI_OFFSET_Y = -0.5 * NINJA_PVC_PILLAR_HEIGHT + NINJA_PVC_REMAIN_X
  + NINJA_PVC_THICK_PILLAR_WIDTH;
// NINJA shifter width/height
const double NINJA_SHIFTER_XY = 130. * cm;
// NINJA shifter depth
const double NINJA_SHIFTER_DEPTH = 3. * cm;
// Thickness of NINJA emulsion layers
const double NINJA_EMULSION_LAYER_THICK = 70. * um;
// Thickness of NINJA base layers
const double NINJA_BASE_LAYER_THICK = 210. * um;
// Thickness of NINJA emusion films
const double NINJA_FILM_THICK = 2 * NINJA_EMULSION_LAYER_THICK + NINJA_BASE_LAYER_THICK;
// Thickness of NINJA nylon envelope
const double NINJA_ENV_NYLON_THICK = 40. * um;
// Thickness of NINJA aluminum envelope
const double NINJA_ENV_AL_THICK = 9. * um;
// Thickness of NINJA polyethylene envelope
const double NINJA_ENV_POLYE_THICK = 60. * um;
// Thickness of NINJA envelope
const double NINJA_ENV_THICK = NINJA_ENV_NYLON_THICK + NINJA_ENV_AL_THICK + NINJA_ENV_POLYE_THICK;
// Thickness of NINJA TSS SS acryl
const double NINJA_TSS_AC_THICK = 1.5 * mm;
// Width of NINJA TSS attach acryl
const double NINJA_TSS_ATTACH_AC_WIDTH = NINJA_TRACKER_XY;
// Height of NINJA TSS attach acryl
const double NINJA_TSS_ATTACH_AC_HEIGHT = 120. * cm;
// Thickness of NINJA TSS attach acryl
const double NINJA_TSS_ATTACH_AC_THICK = 3. * mm;
// Thickness of NINJA water layers
const double NINJA_WATER_LAYER_THICK = 2.3 * mm;
// Thickness of NINJA iron layers
const double NINJA_IRON_LAYER_THICK = 0.5 * mm;
// Thickness of NINJA SS acryl
const double NINJA_SS_AC_THICK = 2. * mm;
// Thickness of NINJA ECC tracking unit
const double NINJA_ECC_UNIT_THICK = 2 * NINJA_ENV_THICK + 2 * NINJA_FILM_THICK + NINJA_IRON_LAYER_THICK;
// Thickness of NINJA SSS unit
const double NINJA_SSS_THICK = 2 * NINJA_ENV_THICK + 4 * NINJA_FILM_THICK + NINJA_SS_AC_THICK;
// Thickness of NINJA SDS unit
const double NINJA_SDS_THICK = 2 * NINJA_ENV_THICK + 2 * NINJA_FILM_THICK + NINJA_SS_AC_THICK;
// Desiccator width
const double NINJA_DESIC_WIDTH = 31. * cm;
// Desiccator height
const double NINJA_DESIC_HEIGHT = 29.5 * cm;
// Desiccator depth
const double NINJA_DESIC_DEPTH = 27. * cm;
// Desiccator thickness
const double NINJA_DESIC_THICK = 1. * cm;
// ECC film width/height
const double NINJA_ECC_FILM_XY = 25. * cm;
// OSS film width
const double NINJA_OSS_FILM_WIDTH = 33. * cm;
// OSS film height
const double NINJA_OSS_FILM_HEIGHT = 28. * cm;
// OSS acryl offset from rack plate
const double NINJA_OSS_ACRYL_OFFSET = 1. * mm;
// Shifter film width
const double NINJA_SHIFTER_FILM_WIDTH = 102. * cm;
// Shifter film height
const double NINJA_SHIFTER_FILM_HEIGHT = 34. * cm;
// Thickness of ECC water spacer bottom
const double NINJA_ECC_WATER_SPACER_BOTTOM = 5. * mm;
// NINJA water fiducial volume width
const double NINJA_FV_WATER_WIDTH = NINJA_ECC_FILM_XY;
// NINJA water fiducial volume height
const double NINJA_FV_WATER_HEIGHT = NINJA_ECC_FILM_XY - NINJA_ECC_WATER_SPACER_BOTTOM;
// NINJA iron fiducial volume width
const double NINJA_FV_IRON_WIDTH = NINJA_ECC_FILM_XY;
// NINJA iron fiducial volume height
const double NINJA_FV_IRON_HEIGHT = NINJA_ECC_FILM_XY;

///////////////////// Detectors position //////////////////////

// Position of the center of each detector with respect to the world coordinates.
// The origin is roughly in the center of the experiment (between proton module and
// WAGASCI downstream)

// X coordinate of the cylindrical B2 wall center
const double WALL_POS_X = (-3.222 + 5.722) * m;
// Y coordinate of the cylindrical B2 wall center
const double WALL_POS_Y = (0. + 4.444) * m;
// Z coordinate of the cylindrical B2 wall center
const double WALL_POS_Z = (1.7 - 4.2) * m;

// X coordinate of the south pillar
const double PILLAR_SOUTH_POS_X = (-8.435 + 5.722 + 0.2) * m;
// Y coordinate of the south pillar
const double PILLAR_SOUTH_POS_Y = (-4.443 + 4.444) * m;
// Z coordinate of the south pillar
const double PILLAR_SOUTH_POS_Z = (2.631 - 4.2) * m;

// X coordinate of the north pillar
const double PILLAR_NORTH_POS_X = (-8.035 + 5.0 + 5.722 + 0.2) * m;
// Y coordinate of the north pillar
const double PILLAR_NORTH_POS_Y = (-4.443 + 4.444) * m;
// Z coordinate of the north pillar
const double PILLAR_NORTH_POS_Z = (2.631 - 4.2) * m;

// X coordinate of the proton module center
//const double PROTON_MODULE_POS_X = -0.35 * m;
const double PROTON_MODULE_POS_X = -0.335 * m;
// Y coordinate of the proton module center
const double PROTON_MODULE_POS_Y = -0.2 * cm;
// Z coordinate of the proton module center
const double PROTON_MODULE_POS_Z = -0.80 * m;

// X coordinate of the WAGASCI upstream center
//const double WAGASCI_UPSTREAM_POS_X = -0.362 * m;
const double WAGASCI_UPSTREAM_POS_X = -0.331 * m;
// Y coordinate of the WAGASCI upstream center
//const double WAGASCI_UPSTREAM_POS_Y = 1.3 * cm;
const double WAGASCI_UPSTREAM_POS_Y = 2.4 * cm;
// Z coordinate of the WAGASCI upstream center
const double WAGASCI_UPSTREAM_POS_Z = -1.557 * m;

// X coordinate of the WAGASCI downstream center
//const double WAGASCI_DOWNSTREAM_POS_X = -9.4* cm;
const double WAGASCI_DOWNSTREAM_POS_X = -6.4* cm;
// Y coordinate of the WAGASCI downstream center
//const double WAGASCI_DOWNSTREAM_POS_Y = 1.3 * cm;
const double WAGASCI_DOWNSTREAM_POS_Y = 0.7 * cm;
// Z coordinate of the WAGASCI downstream center
const double WAGASCI_DOWNSTREAM_POS_Z = 44.5 * cm;

// X coordinate of the WallMRD north center
const double WALLMRD_SOUTH_POS_X = 1.166 * m;
// Y coordinate of both the WallMRDs center
const double WALLMRD_POS_Y = 0.12 * m;
// Z coordinate of the WallMRD north center
const double WALLMRD_SOUTH_POS_Z = -0.095 * m;

// X coordinate of the WallMRD south center
const double WALLMRD_NORTH_POS_X = -1.592 * m;
// Z coordinate of the WallMRD south center
const double WALLMRD_NORTH_POS_Z = -0.50 * m;

// X coordinate of the BabyMIND center
const double BABYMIND_POS_X = 0.45 * m;
// Y coordinate of the BabyMIND center
//const double BABYMIND_POS_Y = 0 * m;
const double BABYMIND_POS_Y = -7.0 * cm;
// Z coordinate of the BabyMIND center
const double BABYMIND_POS_Z = 2.80 * m;

// X coordinate of the NINJA detector center
const double NINJA_POS_X = -30.5 * cm;
// Y coordinate of the NINJA detector center
const double NINJA_POS_Y = -8.9 * cm;
// Z coordinate of the NINJA detector center
const double NINJA_POS_Z = -7.72 * cm;

///////////////////// NINJA Sub-detectors position //////////////////////

// X coordinate of the NINJA rack center (downstream, north pillar)
const double NINJA_RACK_POS_X = - 0.5 * NINJA_BOX_WIDTH + 0.5 * NINJA_RACK_PILLAR_WIDTH;
// Y coordinate of the NINJA rack center (downstream, north pillar)
const double NINJA_RACK_POS_Y = 0. * cm;
// Z coordinate of the NINJA rack center (downstream, north pillar)
const double NINJA_RACK_POS_Z = - 0.5 * NINJA_BOX_DEPTH + NINJA_RACK_PILLAR_DEPTH
  + NINJA_RACK_PLATE_DEPTH + 0.5 * NINJA_RACK_PILLAR_DEPTH;

// X coordinate of the NINJA tracker center
const double NINJA_TRACKER_POS_X = 0. * cm;
// Y coordinate of the NINJA tracker center
// 454.6 cm is modified measured value
const double NINJA_TRACKER_POS_Y = - 0.5 * NINJA_BOX_HEIGHT + 44.6 * cm + 0.5 * NINJA_TRACKER_XY;
// Z coordinate of the NINJA tracker center
const double NINJA_TRACKER_POS_Z = 0.5 * NINJA_BOX_DEPTH - 0.5 * NINJA_TRACKER_DEPTH;

// X coordinate of the NINJA TSS acryl center
const double NINJA_TSS_POS_X = 0. * cm;
// Y coordinate of the NINJA TSS acryl center
const double NINJA_TSS_POS_Y = - 0.5 * NINJA_RACK_HEIGHT + 45. * cm + NINJA_TRACKER_XY - 0.5 * NINJA_TSS_ATTACH_AC_HEIGHT;
// Z coordinate of the NINJA TSS acryl center
const double NINJA_TSS_POS_Z = 0.5 * NINJA_BOX_DEPTH  - NINJA_TRACKER_DEPTH - 0.5 * NINJA_TSS_ATTACH_AC_THICK;

// X coordinate of the NINJA shifter center
const double NINJA_SHIFTER_POS_X = -2. * cm;
// Y coordinate of the NINJA shifter center
const double NINJA_SHIFTER_POS_Y = - 0.5 * NINJA_RACK_HEIGHT + 105. * cm + 0.5 * NINJA_DESIC_HEIGHT;
// Z coordinate of the NINJA shifter center
const double NINJA_SHIFTER_POS_Z = 0.5 * NINJA_BOX_DEPTH - NINJA_TRACKER_DEPTH - NINJA_TSS_AC_THICK
  - 2 * NINJA_ENV_THICK - 4 * NINJA_FILM_THICK - NINJA_TSS_ATTACH_AC_THICK - 0.5 * NINJA_SHIFTER_DEPTH;
// Y gap of shifter/TSS packings
const double NINJA_SHIFTER_FILM_GAP_Y = NINJA_SHIFTER_FILM_HEIGHT + 2 * NINJA_ENV_THICK;

// X coordinate of the NINJA ECC center
const double NINJA_ECC_POS_X = - 7. * cm;
// Y coordinate of the NINJA ECC center
// 100 cm is rack plate height
const double NINJA_ECC_POS_Y = - 0.5 * NINJA_RACK_HEIGHT + 105. * cm + 0.5 * NINJA_DESIC_HEIGHT;
// Z coordinate of the NINJA ECC center
const double NINJA_ECC_POS_Z = NINJA_RACK_POS_Z - 0.5 * NINJA_RACK_PILLAR_DEPTH
  - 2 * NINJA_ENV_THICK - 4 * NINJA_FILM_THICK - 3 * NINJA_SS_AC_THICK - 0.5 * NINJA_DESIC_DEPTH;
// X gap of the NINJA ECCs
const double NINJA_ECC_GAP_X = NINJA_DESIC_WIDTH + NINJA_SSS_THICK + NINJA_SDS_THICK;
// Y gap of the NINJA ECCs
// plate height difference
const double NINJA_ECC_GAP_Y = 35. * cm;

// X coordinate of NINJA water fiducial volume center
const double NINJA_FV_WATER_POS_X = NINJA_ECC_POS_X;
// Y coordinate of NINJA water fiducial volume center
const double NINJA_FV_WATER_POS_Y = - 0.5 * NINJA_RACK_HEIGHT + 105. * cm + NINJA_DESIC_THICK
  + NINJA_ENV_THICK + NINJA_ECC_WATER_SPACER_BOTTOM + 0.5 * NINJA_FV_WATER_HEIGHT;
// X coordinate of NINJA iron fiducial volume center
const double NINJA_FV_IRON_POS_X = NINJA_ECC_POS_X;
// Y coordinate of NINJA iron fiducial volume center
const double NINJA_FV_IRON_POS_Y = - 0.5 * NINJA_RACK_HEIGHT + 105. * cm + NINJA_DESIC_THICK
  + NINJA_ENV_THICK + 0.5 * NINJA_FV_IRON_HEIGHT;

///////////////////// Materials density //////////////////////

const double AIR_DENSITY = 1.29; // mg/cm3
const double IRON_DENSITY = 7.86; // g/cm3
const double WAGASCI_WATER_DENSITY = 0.9963; // g/cm3
const double CONCRETE_DENSITY = 2.2; // g/cm3
const double NINJA_WATER_DENSITY = 1.; // g/cm3
const double SCINTILLATOR_DENSITY = 1.032; // g/cm3

#endif
