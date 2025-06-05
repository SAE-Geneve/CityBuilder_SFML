#ifndef BT_NODE_H
#define BT_NODE_H

namespace core::ai {
    namespace behaviour_tree {

        enum class Status {
            kFailure,
            kRunning,
            kSuccess
        };

        class Node {
        public:
            //virtual ~Node() = default;
            virtual void Reset() = 0;
            virtual Status Tick() = 0;

        protected:
            Status status_ = Status::kFailure;

        };

    }
}

#endif //BT_NODE_H
