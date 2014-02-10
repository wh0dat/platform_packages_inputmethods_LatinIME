/*
 * Copyright (C) 2013, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LATINIME_VER4_PATRICIA_TRIE_NODE_WRITER_H
#define LATINIME_VER4_PATRICIA_TRIE_NODE_WRITER_H

#include <stdint.h>

#include "defines.h"
#include "suggest/policyimpl/dictionary/structure/pt_common/dynamic_pt_reading_helper.h"
#include "suggest/policyimpl/dictionary/structure/pt_common/pt_node_params.h"
#include "suggest/policyimpl/dictionary/structure/pt_common/pt_node_writer.h"
#include "suggest/policyimpl/dictionary/structure/v4/content/probability_entry.h"

namespace latinime {

class BufferWithExtendableBuffer;
class Ver4BigramListPolicy;
class Ver4DictBuffers;
class Ver4PatriciaTrieNodeReader;
class Ver4PtNodeArrayReader;
class Ver4ShortcutListPolicy;

/*
 * This class is used for helping to writes nodes of ver4 patricia trie.
 */
class Ver4PatriciaTrieNodeWriter : public PtNodeWriter {
 public:
    Ver4PatriciaTrieNodeWriter(BufferWithExtendableBuffer *const trieBuffer,
            Ver4DictBuffers *const buffers, const PtNodeReader *const ptNodeReader,
            const PtNodeArrayReader *const ptNodeArrayReader,
            Ver4BigramListPolicy *const bigramPolicy, Ver4ShortcutListPolicy *const shortcutPolicy)
            : mTrieBuffer(trieBuffer), mBuffers(buffers),
              mReadingHelper(ptNodeReader, ptNodeArrayReader), mBigramPolicy(bigramPolicy),
              mShortcutPolicy(shortcutPolicy) {}

    virtual ~Ver4PatriciaTrieNodeWriter() {}

    virtual bool markPtNodeAsDeleted(const PtNodeParams *const toBeUpdatedPtNodeParams);

    virtual bool markPtNodeAsMoved(const PtNodeParams *const toBeUpdatedPtNodeParams,
            const int movedPos, const int bigramLinkedNodePos);

    virtual bool markPtNodeAsWillBecomeNonTerminal(
            const PtNodeParams *const toBeUpdatedPtNodeParams);

    virtual bool updatePtNodeProbability(const PtNodeParams *const toBeUpdatedPtNodeParams,
            const int newProbability, const int timestamp);

    virtual bool updatePtNodeProbabilityAndGetNeedsToKeepPtNodeAfterGC(
            const PtNodeParams *const toBeUpdatedPtNodeParams, bool *const outNeedsToKeepPtNode);

    virtual bool updateChildrenPosition(const PtNodeParams *const toBeUpdatedPtNodeParams,
            const int newChildrenPosition);

    bool updateTerminalId(const PtNodeParams *const toBeUpdatedPtNodeParams,
            const int newTerminalId);

    virtual bool writePtNodeAndAdvancePosition(const PtNodeParams *const ptNodeParams,
            int *const ptNodeWritingPos);

    virtual bool writeNewTerminalPtNodeAndAdvancePosition(const PtNodeParams *const ptNodeParams,
            const int timestamp, int *const ptNodeWritingPos);

    virtual bool addNewBigramEntry(const PtNodeParams *const sourcePtNodeParams,
            const PtNodeParams *const targetPtNodeParam, const int probability, const int timestamp,
            bool *const outAddedNewBigram);

    virtual bool removeBigramEntry(const PtNodeParams *const sourcePtNodeParams,
            const PtNodeParams *const targetPtNodeParam);

    virtual bool updateAllBigramEntriesAndDeleteUselessEntries(
            const PtNodeParams *const sourcePtNodeParams, int *const outBigramEntryCount);

    virtual bool updateAllPositionFields(const PtNodeParams *const toBeUpdatedPtNodeParams,
            const DictPositionRelocationMap *const dictPositionRelocationMap,
            int *const outBigramEntryCount);

    virtual bool addShortcutTarget(const PtNodeParams *const ptNodeParams,
            const int *const targetCodePoints, const int targetCodePointCount,
            const int shortcutProbability);

    bool updatePtNodeHasBigramsAndShortcutTargetsFlags(const PtNodeParams *const ptNodeParams);

 private:
    DISALLOW_COPY_AND_ASSIGN(Ver4PatriciaTrieNodeWriter);

    bool writePtNodeAndGetTerminalIdAndAdvancePosition(
            const PtNodeParams *const ptNodeParams, int *const outTerminalId,
            int *const ptNodeWritingPos);

    // Create updated probability entry using given probability and timestamp. In addition to the
    // probability, this method updates historical information if needed.
    const ProbabilityEntry createUpdatedEntryFrom(
            const ProbabilityEntry *const originalProbabilityEntry, const int newProbability,
            const int timestamp) const;

    bool updatePtNodeFlags(const int ptNodePos, const bool isBlacklisted, const bool isNotAWord,
            const bool isTerminal, const bool hasShortcutTargets, const bool hasBigrams,
            const bool hasMultipleChars);

    static const int CHILDREN_POSITION_FIELD_SIZE;

    BufferWithExtendableBuffer *const mTrieBuffer;
    Ver4DictBuffers *const mBuffers;
    DynamicPtReadingHelper mReadingHelper;
    Ver4BigramListPolicy *const mBigramPolicy;
    Ver4ShortcutListPolicy *const mShortcutPolicy;
};
} // namespace latinime
#endif /* LATINIME_VER4_PATRICIA_TRIE_NODE_WRITER_H */
