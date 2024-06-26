
#include "test_utils.h"

#include <fstream>  // std::ifstream, std::ofstream
#include <array>
#include <utility>
#include <memory>
#include <iostream>

#include <catch2/catch_all.hpp>

#include <bag_dataset.h>
#include <bag_metadata.h>
#include <bag_metadataprofiles.h>
#include <bag_simplelayer.h>
#include <bag_surfacecorrections.h>
#include <bag_surfacecorrectionsdescriptor.h>


using BAG::Dataset;

namespace TestUtils {

constexpr uint32_t kGridSize = 100;
constexpr uint32_t kSepSize = 3;

//! Helper to copy a source file to a destination file.
void copyFile(
    const std::string& source,
    const std::string& dest)
{
    const std::ifstream in{source, std::ios::binary|std::ios::in};
    std::ofstream out{dest, std::ios::binary|std::ios::out};

    out << in.rdbuf();
}

std::pair<std::shared_ptr<BAG::Dataset>, std::string>
    createBag(const std::string metadataFileName,
              const std::string bagFileName) {
    // Load sample metadata
    BAG::Metadata metadata;
    metadata.loadFromFile(metadataFileName);

    // Create the dataset
    constexpr uint64_t chunkSize = 100;
    constexpr int compressionLevel = 1;
    std::shared_ptr<BAG::Dataset> dataset = BAG::Dataset::create(bagFileName, std::move(metadata),
                                   chunkSize, compressionLevel);

    // Write the elevation layer, constructing bogus data as we do so.
    auto elevationLayer = dataset->getSimpleLayer(Elevation);

    // Set the min/max values (optional).
    // NOTE: Layer::write() calls update min/max.
    {
        const std::array<float, 2> surfRange{-10.0f,
                                             -10.0f - ((kGridSize - 1) * (kGridSize - 1) + kGridSize) / 10.0f};

        auto pDescriptor = elevationLayer->getDescriptor();
        pDescriptor->setMinMax(surfRange[0], surfRange[1]);

        elevationLayer->writeAttributes();
    }
    std::string elevationLayerName = elevationLayer->getDescriptor()->getName();

    // Write the data.
    std::array<float, kGridSize> surf{};

    for(uint32_t row=0; row<kGridSize; ++row)
    {
        for (uint32_t column=0; column<kGridSize; ++column)
            surf[column] = ((column * row) % kGridSize) +
                           (column / static_cast<float>(kGridSize));

        try
        {
            const auto* buffer = reinterpret_cast<uint8_t*>(surf.data());
            constexpr uint32_t columnStart = 0;
            constexpr uint32_t columnEnd = kGridSize - 1;

            elevationLayer->write(row, columnStart, row, columnEnd, buffer);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            FAIL(e.what());
        }
    }

    // Write the uncertainty layer, constructing bogus data as we do so.
    auto uncertaintyLayer = dataset->getSimpleLayer(Uncertainty);

    // Set the min/max values (optional).
    // NOTE: Layer::write() calls update min/max.
    {
        const std::array<float, 2> uncertRange{1.0f,
                                               1.0f + ((kGridSize - 1) * (kGridSize - 1) + kGridSize) / 100.0f};

        auto pDescriptor = uncertaintyLayer->getDescriptor();
        pDescriptor->setMinMax(uncertRange[0], uncertRange[1]);

        uncertaintyLayer->writeAttributes();
    }

    // Write the data.
    {
        std::array<float, kGridSize> uncert{};

        for (uint32_t row = 0; row < kGridSize; ++row) {
            for (uint32_t column = 0; column < kGridSize; ++column)
                uncert[column] = ((column * row) % kGridSize) / 1000.0f;

            try {
                const auto *buffer = reinterpret_cast<uint8_t *>(uncert.data());
                constexpr uint32_t columnStart = 0;
                constexpr uint32_t columnEnd = kGridSize - 1;

                uncertaintyLayer->write(row, columnStart, row, columnEnd, buffer);
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << '\n';
                FAIL(e.what());
            }
        }
    }

    // Write the uncertainty layer, constructing bogus data as we do so.
    uncertaintyLayer = dataset->getSimpleLayer(Uncertainty);

    // Set the min/max values (optional).
    // NOTE: Layer::write() calls update min/max.
    {
        const std::array<float, 2> uncertRange{1.0f,
                                               1.0f + ((kGridSize - 1) * (kGridSize - 1) + kGridSize) / 100.0f};

        auto pDescriptor = uncertaintyLayer->getDescriptor();
        pDescriptor->setMinMax(uncertRange[0], uncertRange[1]);

        uncertaintyLayer->writeAttributes();
    }

    // Write the data.
    {
        std::array<float, kGridSize> uncert{};

        for (uint32_t row = 0; row < kGridSize; ++row) {
            for (uint32_t column = 0; column < kGridSize; ++column)
                uncert[column] = ((column * row) % kGridSize) / 1000.0f;

            try {
                const auto *buffer = reinterpret_cast<uint8_t *>(uncert.data());
                constexpr uint32_t columnStart = 0;
                constexpr uint32_t columnEnd = kGridSize - 1;

                uncertaintyLayer->write(row, columnStart, row, columnEnd, buffer);
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << '\n';
                FAIL(e.what());
            }
        }
    }

    return std::pair<std::shared_ptr<BAG::Dataset>, std::string>{dataset, elevationLayerName};
}

void create_NOAA_OCS_Metadata(const std::string& simpleLayerName,
                              std::shared_ptr<BAG::Dataset> dataset) {
    try
    {
        constexpr uint64_t chunkSize = 100;
        constexpr unsigned int compressionLevel = 1;
        auto& compoundLayer = dataset->createGeorefMetadataLayer(
                BAG::GeorefMetadataProfile::NOAA_OCS_2022_10_METADATA_PROFILE,
                simpleLayerName, chunkSize, compressionLevel);

        // At this point, all entries in the georeferenced metadata layer point to index 0,
        // which is a no data value.

        // Write a couple records.

        using BAG::CompoundDataType;

        BAG::Record record = BAG::CreateRecord_NOAA_OCS_2022_10(
                false,     // significant_features
                true,      // feature_least_depth
                1234.567f,      // feature_size
                765.4321f,   // feature_size_var
                true,            // coverage
                false,       // bathy_coverage
                9.87f,  // horizontal_uncert_fixed
                1.23f,   // horizontal_uncert_var
                std::string{"2019-04-01 00:00:00.0Z"}, // survey_data_start
                std::string{"2019-04-01 12:00:00.0Z"},  // survey_date_end
                std::string("NOAA"),                   // source_institution
                std::string("CD71EB77-5812-4735-B728-0DC1AE2A2F3B"),         // source_survey_id
                42,                                                           // source_survey_index
                std::string("Creative Commons Zero Public Domain Dedication (CC0)"),  // license_same
                std::string("https://creativecommons.org/publicdomain/zero/1.0/")      // license_url
        );

        auto& valueTable = compoundLayer.getValueTable();

        // Store the new record in memory and in the BAG.
        const auto firstRecordIndex = valueTable.addRecord(record);

        record = BAG::CreateRecord_NOAA_OCS_2022_10(
                true,         // significant_features
                false,        // feature_least_depth
                987.6f,            // feature_size
                6.789f,         // feature_size_var
                false,              // coverage
                true,           // bathy_coverage
                12345.67f, // horizontal_uncert_fixed
                89.0f,      // horizontal_uncert_var
                std::string{"2019-04-02 00:00:00.0Z"},  // survey_data_start
                std::string{"2019-04-02 12:00:00.0Z"},   // survey_date_end
                std::string("NOAA"),                    // source_institution
                std::string("15B46F99-1D94-4669-92D8-AA86F533B097"),         // source_survey_id
                23,                                                           // source_survey_index
                std::string("Open Data Commons Public Domain Dedication and Licence (PDDL)"), // license_name
                std::string("http://opendatacommons.org/licenses/pddl/1.0/")     // license_url
        );

        // Store the new record in memory and in the BAG.
        const auto secondRecordIndex = valueTable.addRecord(record);

        uint32_t numRows = 0;
        uint32_t numColumns = 0;
        std::tie(numRows, numColumns) = dataset->getDescriptor().getDims();

        // Set up the georeferenced metadata layer to point to the new records.
        // Let's say the first 5 rows of elevation should use the first record
        // index, and the next 3 columns use the second record index.

        // Start at row 0, go to (including) row 4.
        // Use the entire column.
        uint32_t rowStart = 0;
        uint32_t columnStart = 0;
        uint32_t rowEnd = 4;
        uint32_t columnEnd = numColumns - 1;

        // Create the buffer.  The type depends on the indexType used when
        // creating the georeferenced metadata layer.
        // The buffer contains the first record's index covering the first four
        // rows (across all the columns).
        size_t numElements = (rowEnd - rowStart + 1) * numColumns;
        const std::vector<uint16_t> firstBuffer(numElements, (uint16_t)firstRecordIndex);

        compoundLayer.write(rowStart, columnStart, rowEnd, columnEnd,
                            reinterpret_cast<const uint8_t*>(firstBuffer.data()));

        // Start at row 6, go to the last row.
        // Start at column 0, go to (including) column 2.
        rowStart = 5;
        columnStart = 0;
        rowEnd = numRows - 1;
        columnEnd = 2;

        // Create the buffer.  The type depends on the indexType used when
        // creating the georeferenced metadata layer.
        // The buffer contains the second record's index covering the first four
        // rows (across all the columns).
        numElements = (rowEnd - rowStart + 1) * (columnEnd - columnStart + 1);
        const std::vector<uint16_t> secondBuffer(numElements, (uint16_t)secondRecordIndex);

        compoundLayer.write(rowStart, columnStart, rowEnd, columnEnd,
                            reinterpret_cast<const uint8_t*>(secondBuffer.data()));

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        FAIL(e.what());
    }
}

void create_unknown_metadata(const std::string& elevationLayerName,
                             const std::shared_ptr<BAG::Dataset>& dataset) {
    // Create georeferenced metadata layer of unknown metadata profile
    BAG::RecordDefinition definition(2);
    definition[0].name = "dummy_int";
    definition[0].type = DT_UINT32;
    definition[1].name = "dummy_float";
    definition[1].type = DT_FLOAT32;

    constexpr uint64_t chunkSize = 100;
    constexpr unsigned int compressionLevel = 1;

    const BAG::DataType indexType = DT_UINT16;
    auto &compoundLayer = dataset->createGeorefMetadataLayer(indexType, UNKNOWN_METADATA_PROFILE,
                                                       elevationLayerName, definition, chunkSize,
                                                       compressionLevel);
    auto &valueTable = compoundLayer.getValueTable();
    // Add some records to the georeferenced metadata layer
    using BAG::CompoundDataType;
    // First record
    BAG::Record record{
            CompoundDataType{1u},  // dummy_int
            CompoundDataType{123.456f},  // dummy_float
    };
    const auto firstRecordIndex = valueTable.addRecord(record);
    // Second record
    record = BAG::Record{
            CompoundDataType{2u},  // dummy_int
            CompoundDataType{456.123f},  // dummy_float
    };
    const auto secondRecordIndex = valueTable.addRecord(record);

    // Write record index values to georeferenced metadata layer raster
    uint32_t numRows = 0;
    uint32_t numColumns = 0;
    std::tie(numRows, numColumns) = dataset->getDescriptor().getDims();

    // Set up the georeferenced metadata layer to point to the new records.
    // Let's say the first 5 rows of elevation should use the first record
    // index, and the next 3 columns use the second record index.

    // Start at row 0, go to (including) row 4.
    // Use the entire column.
    uint32_t rowStart = 0;
    uint32_t columnStart = 0;
    uint32_t rowEnd = 4;
    uint32_t columnEnd = numColumns - 1;

    // Create the buffer.  The type depends on the indexType used when
    // creating the georeferenced metadata layer.
    // The buffer contains the first record's index covering the first four
    // rows (across all the columns).
    size_t numElements = (rowEnd - rowStart + 1) * numColumns;
    const std::vector<uint16_t> firstBuffer(numElements, (uint16_t)firstRecordIndex);

    compoundLayer.write(rowStart, columnStart, rowEnd, columnEnd,
                        reinterpret_cast<const uint8_t *>(firstBuffer.data()));

    // Start at row 6, go to the last row.
    // Start at column 0, go to (including) column 2.
    rowStart = 5;
    columnStart = 0;
    rowEnd = numRows - 1;
    columnEnd = 2;

    // Create the buffer.  The type depends on the indexType used when
    // creating the georeferenced metadata layer.
    // The buffer contains the second record's index covering the first four
    // rows (across all the columns).
    numElements = (rowEnd - rowStart + 1) * (columnEnd - columnStart + 1);
    const std::vector<uint16_t> secondBuffer(numElements, (uint16_t)secondRecordIndex);

    compoundLayer.write(rowStart, columnStart, rowEnd, columnEnd,
                        reinterpret_cast<const uint8_t *>(secondBuffer.data()));
}

}  // namespace TestUtils

