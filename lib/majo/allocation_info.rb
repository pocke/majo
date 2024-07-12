module Majo
  class AllocationInfo
    def inspect
      "#<Majo::AllocationInfo #{object_class_path} #{class_path}##{method_id} #{path}:#{line} gen#{alloc_generation}..#{free_generation}>"
    end

    def hash
      path.hash ^ class_path.hash ^ method_id.hash ^ line.hash ^ object_class_path.hash ^ alloc_generation.hash ^ free_generation.hash ^ memsize.hash
    end

    def eql?(other)
      other.is_a?(AllocationInfo) &&
        path == other.path &&
        class_path == other.class_path &&
        method_id == other.method_id &&
        line == other.line &&
        object_class_path == other.object_class_path &&
        alloc_generation == other.alloc_generation &&
        free_generation == other.free_generation &&
        memsize == other.memsize
    end
  end
end
