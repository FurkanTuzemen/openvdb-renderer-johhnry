#include "VDBFile.hpp"
#include <openvdb/Grid.h>
#include <openvdb/io/Stream.h>
#include <openvdb/math/Coord.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/ChangeBackground.h>
#include <openvdb/tools/Composite.h>
#include <openvdb/tools/LevelSetSphere.h>
#include <openvdb/tools/RayTracer.h>
#include <openvdb/tools/VolumeToMesh.h>

using namespace openvdb::OPENVDB_VERSION_NAME::tools;

#define FT_CHRONO_BEGIN std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define FT_CHRONO_END                                                                                                  \
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();                                      \
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()    \
              << "[ms]" << std::endl;

#include <string>
#include <vector>

#define GLEW_NO_GLU
#include <GL/glew.h>

#include <imgui/imgui.h>

#include "geometry/CubeGeometry.hpp"
#include "log/Log.hpp"

openvdb::FloatGrid::Ptr createSphereGrid(float radius, openvdb::Vec3f center, float voxelSize) {
    // Create a level set sphere grid
    openvdb::FloatGrid::Ptr grid = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(radius, center, voxelSize);

    return grid;
}

std::string getGridType(openvdb::GridBase::Ptr grid) {
    if (grid->isType<openvdb::BoolGrid>())
        return "BoolGrid";
    else if (grid->isType<openvdb::FloatGrid>())
        return "FloatGrid";
    else if (grid->isType<openvdb::DoubleGrid>())
        return "DoubleGrid";
    else if (grid->isType<openvdb::Int32Grid>())
        return "Int32Grid";
    else if (grid->isType<openvdb::Int64Grid>())
        return "Int64Grid";
    else if (grid->isType<openvdb::Vec3IGrid>())
        return "Vec3IGrid";
    else if (grid->isType<openvdb::Vec3SGrid>())
        return "Vec3SGrid";
    else if (grid->isType<openvdb::Vec3DGrid>())
        return "Vec3DGrid";

    return "Type not found";
}

VDBFile::VDBFile(std::string filename) : filename(filename) {
    Log::info("Creating .vdb file " + filename);

    file = new openvdb::io::File(filename);
    file->open();
    Log::success("Opened VDB file " + filename);

    // Iterate over grid names
    for (openvdb::io::File::NameIterator nameIter = file->beginName(); nameIter != file->endName(); ++nameIter) {

        // Get grid name and read it from file
        std::string gridName            = nameIter.gridName();
        openvdb::GridBase::Ptr baseGrid = file->readGrid(gridName);

        gridNames.push_back(gridName + " (" + getGridType(baseGrid) + ")");
        grids.push_back(baseGrid);

        Log::message(nameIter.gridName());
    }
}

VDBFile::~VDBFile() noexcept {
    file->close();
}

void VDBFile::displayMetaData(int gridIndex) {
    if (ImGui::BeginTable("MetaData", 2)) {
        ImGui::TableSetupColumn("Key");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        openvdb::GridBase::Ptr grid = grids.at(gridIndex);

        for (openvdb::MetaMap::MetaIterator iter = grid->beginMeta(); iter != grid->endMeta(); ++iter) {
            const std::string &name      = iter->first;
            openvdb::Metadata::Ptr value = iter->second;

            ImGui::TableNextColumn();
            ImGui::Text("%s", name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", value->str().c_str());
        }

        ImGui::EndTable();
    }
}

void VDBFile::constructPointCloud(int gridIndex) {
    // Remove previous cubes
    cubes.clear();

    using GridType     = openvdb::FloatGrid;
    GridType::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(grids.at(gridIndex));

    int nbVoxels = 0;
    int nbPut    = 0;
    for (GridType::ValueOnCIter iter = grid->cbeginValueOn(); iter.test(); ++iter) {
        if (iter.isVoxelValue() && nbVoxels % 100 == 0) {
            openvdb::math::Coord pt = iter.getCoord();

            IGeometry *cube = new CubeGeometry(pt.x() / 100.0, pt.y() / 100.0, pt.z() / 100.0, 1.0);
            cubes.emplace_back(std::unique_ptr<IGeometry>(cube));

            nbPut++;
        }

        nbVoxels++;
    }

    Log::success("Created " + std::to_string(nbPut) + " cubes!");
}

void VDBFile::constructPointCloud(const openvdb::FloatGrid *grid) {
    // Remove previous cubes
    cubes.clear();

    int nbVoxels = 0;
    int nbPut    = 0;
    for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter.test(); ++iter) {
        if (iter.isVoxelValue() && nbVoxels % 100 == 0) {
            openvdb::math::Coord pt = iter.getCoord();

            IGeometry *cube = new CubeGeometry(pt.x() / 100.0, pt.y() / 100.0, pt.z() / 100.0, 1.0);
            cubes.emplace_back(std::unique_ptr<IGeometry>(cube));

            nbPut++;
        }

        nbVoxels++;
    }

    Log::success("Created " + std::to_string(nbPut) + " cubes!");
}

void VDBFile::draw(Program &displayProgram, float scale) {
    // Draw cubes
    static auto sphereGrid1 = createSphereGrid(50.0, openvdb::Vec3f(0.0), 1.f);
    static float i          = -50.f;
    FT_CHRONO_BEGIN
    auto sphereGrid2 = createSphereGrid(30.0, openvdb::Vec3f(0.0f + i, 30, 45), 1.f);
    openvdb::tools::csgDifference(*sphereGrid1, *sphereGrid2);
    i += .1f;

    constructPointCloud(openvdb::gridPtrCast<openvdb::FloatGrid>(sphereGrid1).get());
    for (size_t i = 0; i < cubes.size(); i++) {
        cubes.at(i)->setScale(scale);
        cubes.at(i)->draw(displayProgram);
    }

    FT_CHRONO_END
}

std::vector<std::string> &VDBFile::getGridNames() {
    return gridNames;
}
